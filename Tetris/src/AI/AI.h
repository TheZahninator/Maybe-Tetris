#pragma once

#include "AIController.h"
#include "../GameObjects/Field.h"

class Field;

class AI
{
public:
	AI(Field& field);
	~AI();

	AI* recreate(AI* partner = nullptr);

	void update();
	//void learn(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyState);
	void learn(KeyboardStateTracker* keyboardTracker);

	void updateFitness(unsigned fitness){ m_fitness = fitness; if (m_fitness > m_highscore) m_highscore = m_fitness; }

	unsigned m_fitness;
	unsigned m_highscore;

	std::unique_ptr<ZahnAI::NeuralNet>& getNet(){ return m_net; }

	static std::shared_ptr<AIController> m_controller;
	static unsigned MemorySize;
private:
	std::unique_ptr<ZahnAI::NeuralNet> m_net;

	Field& m_field;
};

