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
	void learn(KeyboardStateTracker* keyboardTracker);
	
	std::vector<double> getInputData();

	void updateFitness(unsigned fitness){ m_fitness = fitness; if (m_fitness > m_highscore) m_highscore = m_fitness; }

	KeyboardStateTracker& getKeyboardStateTracker(){ return m_keyboardStateTracker; }

	unsigned m_fitness;
	unsigned m_highscore;

	std::unique_ptr<ZahnAI::NeuralNet>& getNet(){ return m_net; }

	std::shared_ptr<AIController> m_controller;

	static unsigned MemorySize;
	const static unsigned NumButtons = 6;
private:
	std::unique_ptr<ZahnAI::NeuralNet> m_net;

	Field& m_field;
	KeyboardStateTracker m_keyboardStateTracker;
};

