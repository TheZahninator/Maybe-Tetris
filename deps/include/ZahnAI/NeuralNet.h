#pragma once

#include "Neuron.h"

namespace ZahnAI{

	class NeuralNet
	{
	public:
		NeuralNet(const std::vector<unsigned int> &topology);
		~NeuralNet();

		void feedForward(const std::vector<double> &inputVals);
		void backProp(const std::vector<double> &targetVals);
		void getResults(std::vector<double> &resultVals);

		void train(const std::vector<double> &inputVals, const std::vector<double> &targetVals);
		const std::vector<double> predict();

		void setTrainingMode(bool b){ m_isTraining = b; Neuron::setTraining(b); }
		void setETA(double eta){ m_eta = eta; }
		void setAlpha(double alpha){ m_alpha = alpha; }

		void mutate(double mutationRate);
		NeuralNet* clone();
		NeuralNet* crossover(NeuralNet* partner);

		unsigned getLayerCount(){ return m_layers.size(); }
		Layer& getLayer(unsigned x){ return m_layers[x]; }

	private:
		bool m_isTraining;
		double m_error;

		std::vector<Layer> m_layers;

		double m_eta;
		double m_alpha;
	};

}

struct TrainingData{
	std::vector<double> input;
	std::vector<double> target;
};