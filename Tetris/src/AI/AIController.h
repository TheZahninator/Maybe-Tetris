#pragma once
class AIController
{
public:
	AIController(KeyboardStateTracker& keyboardStateTracker);
	~AIController();

	void reset();

	void rotateCCW(){ m_keyboardStateTracker.press(sf::Keyboard::A); }
	void rotateCW(){ m_keyboardStateTracker.press(sf::Keyboard::D); }
	void pressLeft(){ m_keyboardStateTracker.press(sf::Keyboard::Left); }
	void pressRight(){ m_keyboardStateTracker.press(sf::Keyboard::Right); }
	void pressUp(){ m_keyboardStateTracker.press(sf::Keyboard::Up); }
	void pressDown(){ m_keyboardStateTracker.press(sf::Keyboard::Down); }

private:
	KeyboardStateTracker& m_keyboardStateTracker;
};

