// AI-Trainer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

namespace fs = std::experimental::filesystem;

#define QUEUE_SIZE 8

struct ReplayData {
	unsigned frameNumber;
	bool pressedButtons[6];
	bool newTetromino;
	std::byte queue[QUEUE_SIZE];

	std::vector<double> inputData;
};

ZahnNN::NeuralNet loadNetwork(const wchar_t* path);
std::map<unsigned, ReplayData> loadReplayData(const wchar_t* path, unsigned& data_count);
void saveNetwork(ZahnNN::NeuralNet& net, const wchar_t* path);

int _tmain(int argc, _TCHAR* argv[])
{
	ZahnNN::Neuron::DefaultActivationFunction = ZahnNN::Neuron::ActivationFastSigmoid;
	ZahnNN::Neuron::DefaultActivationFunctionDerivative = ZahnNN::Neuron::ActivationFastSigmoidDerivative;

	std::cout << "Tetris AI trainer" << std::endl << std::endl;

	std::cout << "Place the replay files you want to train with in the \"./res/replays/\" folder." << std::endl;
	std::cout << "Place the neural network save file in the \"./res/\" folder." << std::endl << std::endl;

	std::cout << "When you're ready, press enter..." << std::endl;
	std::getchar();

	const wchar_t* netPath = L"res/save.xml";

	// Load neural net file from './res/' folder
	std::ifstream file(netPath);
	if (!file.good()) {
		std::cout << "No neural network file found!" << std::endl;
		std::getchar();
		file.close();
		exit(0);
	}
	file.close();

	ZahnNN::NeuralNet net = loadNetwork(netPath);

	// Iterate through files in './res/replays/' folder
	std::string replaysPath = "res/replays/";

	auto it = fs::directory_iterator(replaysPath);

	std::cout << "Training..." << std::endl;

	unsigned n = 0;
	for (auto& f : it)
	{
		std::cout << "file " << ++n << std::endl;

		auto path = f.path().c_str();
		unsigned data_count = 0;

		auto data = loadReplayData(path, data_count);

		// Train the network
		for (unsigned i = 0; i < data_count; i++) {
			ReplayData currentData = data[i];

			std::cout << ".";
			if (i+1 % 100 == 0) {
				printf(" %d / %d\n", i, data_count);
			}

			std::vector<double> targetVals;
			targetVals.clear();

			for (unsigned j = 0; j < 6; j++) {
				targetVals.push_back(currentData.pressedButtons[j] ? 1.0 : 0.0);
			}

			net.train(currentData.inputData, targetVals);
		}

		printf(" %d / %d\n", data_count, data_count);

		std::cout << std::endl;
	}

	saveNetwork(net, L"res/save_trained.xml");
	std::cout << "Saved" << std::endl;

	std::getchar();

	return 0;
}

std::map<unsigned, ReplayData> loadReplayData(const wchar_t* path, unsigned& data_count) {
	std::wcout << "Replay: " << path << std::endl;

	pugi::xml_document doc;
	doc.load_file(path);

	pugi::xml_node root = doc.child("replay");

	pugi::xml_attribute atr = root.attribute("data_count");
	data_count = atr.as_uint();

	atr = root.attribute("score");
	unsigned score = atr.as_uint();

	atr = root.attribute("lines");
	unsigned lines = atr.as_uint();

	std::cout << "Data count: " << data_count << std::endl;
	std::cout << "Score: " << score << std::endl;
	std::cout << "Lines: " << lines << std::endl;

	std::map<unsigned, ReplayData> data;
	data.clear();

	// Load training data
	for (pugi::xml_node data_node : root.children("data"))
	{
		ReplayData newData;

		atr = data_node.attribute("frame");
		unsigned frame = atr.as_uint();
		newData.frameNumber = frame;

		atr = data_node.attribute("spawned_new");
		bool spawned_new = atr.as_bool();
		newData.newTetromino = spawned_new;

		pugi::xml_node buttons_node = data_node.child("buttons");

		for (pugi::xml_node button_node : buttons_node.children("button"))
		{
			//std::cout << '.';

			atr = button_node.attribute("index");
			unsigned index = atr.as_uint();

			atr = button_node.attribute("pressed");
			bool pressed = atr.as_bool();

			newData.pressedButtons[index] = pressed;
		}
		//std::cout << std::endl;

		pugi::xml_node queue_node = data_node.child("queue");
		for (pugi::xml_node tetromino_node : queue_node.children("tetromino"))
		{
			//std::cout << '.';

			atr = tetromino_node.attribute("index");
			unsigned index = atr.as_uint();

			atr = tetromino_node.attribute("type");
			int type = atr.as_int();

			newData.queue[index] = static_cast<std::byte>(type);
		}
		//std::cout << std::endl;

		pugi::xml_node input_node = data_node.child("input");

		for (pugi::xml_node value_node : input_node.children("value"))
		{
			//std::cout << '.';

			atr = value_node.attribute("val");
			double value = atr.as_double();

			newData.inputData.push_back(value);
		}
		//std::cout << std::endl;

		data[frame] = newData;
	}

	return data;
}



void saveNetwork(ZahnNN::NeuralNet& net, const wchar_t* path)
{
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("root");

	pugi::xml_node ai = root.append_child("AI");

	for (unsigned j = 0; j < net.getLayerCount(); j++) {
		ZahnNN::Layer& layer = net.getLayer(j);
		pugi::xml_node layerNode = ai.append_child("Layer");

		bool isOutputLayer = j == net.getLayerCount() - 1;

		pugi::xml_attribute size = layerNode.append_attribute("size");
		size.set_value(layer.size() - (unsigned)!isOutputLayer);

		for (unsigned k = 0; k < layer.size() - (unsigned)isOutputLayer; k++) {
			ZahnNN::Neuron& neuron = layer[k];
			pugi::xml_node neuronNode = layerNode.append_child("neuron");

			for (unsigned l = 0; l < neuron.getOutputWeights().size(); l++) {
				double weight = neuron.getOutputWeights()[l].weight;
				pugi::xml_attribute weightAtr = neuronNode.append_attribute("weight");
				weightAtr.set_value(weight);
			}
		}

		if (!isOutputLayer) {
			ZahnNN::Neuron& bias = layer.back();
			pugi::xml_node biasNode = layerNode.append_child("bias");

			for (unsigned l = 0; l < bias.getOutputWeights().size(); l++) {
				double weight = bias.getOutputWeights()[l].weight;
				pugi::xml_attribute weightAtr = biasNode.append_attribute("weight");
				weightAtr.set_value(weight);
			}
		}
	}

	doc.save_file(path);
}

ZahnNN::NeuralNet loadNetwork(const wchar_t* path) {
	pugi::xml_document doc;
	doc.load_file(path);

	pugi::xml_node root = doc.child("root");

	std::vector<unsigned> topo;

	pugi::xml_node ai = root.child("AI");

	for (auto layer : ai.children("Layer"))
	{
		topo.push_back(layer.attribute("size").as_uint());
		std::cout << "Layer size: " << topo.back() << std::endl;
	}

	ZahnNN::NeuralNet net(topo);

	int i = 0;
	for (auto layer : ai.children("Layer"))
	{
		int j = 0;
		for (auto neuron : layer.children("neuron"))
		{
			int k = 0;
			for (auto weight : neuron.attributes())
			{
				net.getLayer(i)[j].getOutputWeights()[k].weight = weight.as_double();

				k++;
			}
			j++;
		}

		for (auto bias : layer.children("bias"))
		{

			int k = 0;
			for (auto weight : bias.attributes())
			{
				net.getLayer(i).back().getOutputWeights()[k].weight = weight.as_double();

				k++;
			}
			j++;
		}
		i++;
	}

	return net;
}