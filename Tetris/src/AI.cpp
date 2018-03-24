#include "pch.h"
#include "AI.h"
#include "Field.h"

std::shared_ptr<AIController> AI::m_controller;

unsigned AI::MemorySize = 10;

AI::AI() : m_fitness(0){

	std::vector<unsigned> topo;
	topo.push_back( 2 + 2 + 1 + 2 + QUEUE_SIZE + MemorySize);
	//topo.push_back(128);
	//topo.push_back(128);
	topo.push_back(64);
	topo.push_back(6 + MemorySize);

	m_net.reset(new ZahnAI::NeuralNet(topo));
	m_net->setETA(0.2);
	m_net->setAlpha(0.5);

	if (!m_controller)
		m_controller.reset(new AIController());
}


AI::~AI(){
}

void AI::update(){
	m_controller->reset();
	//m_fitness++;

	std::vector<double> input;
	input.clear();
	
	//Input the field data
	//for (unsigned i = 0; i < Field::getWidth() * Field::getHeight(); i++){
	//	double d = Field::getGrid()[i] ? 1.0 : 0.0;
	//	input.push_back(d);
	//}

	//Active tetromino position
	double x = Field::getCurrentTetromino()->getPosition().x;
	double y = Field::getCurrentTetromino()->getPosition().y;

	input.push_back(x);
	input.push_back(y);

	//Active Tetromino size
	double w = Field::getCurrentTetromino()->getMatrixSize().x;
	double h = Field::getCurrentTetromino()->getMatrixSize().y;

	input.push_back(w);
	input.push_back(h);

	//Active Tetromino matrix data
	//int newMat[5 * 5];
	//memset(newMat, 0, 5 * 5 * sizeof(int));
	//
	//int* originalMat = Field::getCurrentTetromino()->getMatrix();
	//
	//for (unsigned i = 0; i < (unsigned)w; i++){
	//	for (unsigned j = 0; j < (unsigned)h; j++){
	//		newMat[j * 5 + i] = originalMat[j * (unsigned)w + i];
	//	}
	//}
	//
	//for (unsigned i = 0; i < 5 * 5; i++){
	//	double d = newMat[i] > 0 ? 1.0 : 0.0;
	//	input.push_back(d);
	//}

	//Type
	for (unsigned i = 0; i < QUEUE_SIZE; i++)
		input.push_back((double)Field::getQueue()[i]->getType());

	//Rotation
	input.push_back((double)Field::getCurrentTetromino()->getRotation());

	//Estimated thouching blocks
	input.push_back((double)Field::getCurrentTetromino()->getEstimatedTouchingBlocks());

	//Touching blocks
	input.push_back((double)Field::getCurrentTetromino()->getTouchingBlocks());

	//Memory
	std::vector<double> tmp;
	m_net->getResults(tmp);

	for (unsigned i = 0; i < MemorySize; i++){
		input.push_back(tmp[tmp.size() - MemorySize + i]);
	}

	m_net->feedForward(input);

	std::vector<double> result;
	m_net->getResults(result);

	const double activationWall = 0.75;

	//if (result[0] > activationWall)
	//	m_controller->pressUp();
	if (result[1] > activationWall)
		m_controller->pressDown();
	if (result[2] > activationWall)
		m_controller->pressLeft();
	if (result[3] > activationWall)
		m_controller->pressRight();

	if (result[4] > activationWall)
		m_controller->pressX();
	if (result[5] > activationWall)
		m_controller->pressY();
}

AI* AI::recreate(AI* partner){
	AI* child = new AI();

	if (!partner)
		child->m_net.reset(m_net->clone());
	else
		child->m_net.reset(m_net->crossover(partner->m_net.get()));
	

	double num = (double)rand();
	num = ZAHN_MAP(num, 0, RAND_MAX, 1.0, 10.0);
	child->m_net->mutate(num);

	return child;
}


void AI::learn(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyState){
	std::vector<double> input;
	input.clear();

	//Input the field data
	//for (unsigned i = 0; i < Field::getWidth() * Field::getHeight(); i++){
	//	double d = Field::getGrid()[i] ? 1.0 : 0.0;
	//	input.push_back(d);
	//}

	//Active tetromino position
	double x = Field::getCurrentTetromino()->getPosition().x;
	double y = Field::getCurrentTetromino()->getPosition().y;

	input.push_back(x);
	input.push_back(y);

	//Active Tetromino size
	double w = Field::getCurrentTetromino()->getMatrixSize().x;
	double h = Field::getCurrentTetromino()->getMatrixSize().y;

	input.push_back(w);
	input.push_back(h);

	//Active Tetromino matrix data
	//int newMat[5 * 5];
	//memset(newMat, 0, 5 * 5 * sizeof(int));
	//
	//int* originalMat = Field::getCurrentTetromino()->getMatrix();
	//
	//for (unsigned i = 0; i < (unsigned)w; i++){
	//	for (unsigned j = 0; j < (unsigned)h; j++){
	//		newMat[j * 5 + i] = originalMat[j * (unsigned)w + i];
	//	}
	//}
	//
	//for (unsigned i = 0; i < 5 * 5; i++){
	//	double d = newMat[i] > 0 ? 1.0 : 0.0;
	//	input.push_back(d);
	//}

	//Type
	for (unsigned i = 0; i < QUEUE_SIZE; i++)
		input.push_back((double)Field::getQueue()[i]->getType());

	//Rotation
	input.push_back((double)Field::getCurrentTetromino()->getRotation());

	//Estimated thouching blocks
	input.push_back((double)Field::getCurrentTetromino()->getEstimatedTouchingBlocks());

	//Touching blocks
	input.push_back((double)Field::getCurrentTetromino()->getTouchingBlocks());

	//Memory
	std::vector<double> tmp;
	m_net->getResults(tmp);

	for (unsigned i = 0; i < MemorySize; i++){
		input.push_back(tmp[tmp.size() - MemorySize + i]);
	}

	m_net->feedForward(input);

	//Get the target values for the button presses(player input)
	std::vector<double> targetVals;
	targetVals.clear();

	double d = 0.0;
	d = double(inputGamePad->dpadUp == inputGamePad->PRESSED || keyboardTracker->IsKeyPressed(DirectX::Keyboard::Up));
	targetVals.push_back(d);

	d = double(inputGamePad->dpadDown == inputGamePad->HELD || keyState->Down);
	targetVals.push_back(d);

	d = double(inputGamePad->dpadLeft == inputGamePad->PRESSED || keyboardTracker->IsKeyPressed(DirectX::Keyboard::Left));
	targetVals.push_back(d);

	d = double(inputGamePad->dpadRight == inputGamePad->PRESSED || keyboardTracker->IsKeyPressed(DirectX::Keyboard::Right));
	targetVals.push_back(d);

	d = double(inputGamePad->x == inputGamePad->PRESSED || keyboardTracker->IsKeyPressed(DirectX::Keyboard::A));
	targetVals.push_back(d);

	d = double(inputGamePad->y == inputGamePad->PRESSED || keyboardTracker->IsKeyPressed(DirectX::Keyboard::D));
	targetVals.push_back(d);

	m_net->backProp(targetVals);
}