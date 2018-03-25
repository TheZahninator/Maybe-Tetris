#pragma once
class AIController : public sf::Keyboard
{
public:
	AIController();
	~AIController();
/*
	void reset();

	void pressX(){ .buttons.x = true; }
	void pressY(){ m_state.buttons.y = true; }
	void pressLeft(){ m_state.dpad.left = true; }
	void pressRight(){ m_state.dpad.right = true; }
	void pressUp(){ m_state.dpad.up = true; }
	void pressDown(){ m_state.dpad.down = true; }
*/
	//State GetState();

private:
	//State m_state;
};

