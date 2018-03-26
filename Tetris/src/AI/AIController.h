#pragma once
class AIController
{
public:
	AIController(KeyboardStateTracker* keyboardStateTracker);
	~AIController();

	void reset();

	void rotateCCW();
	void rotateCW();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

private:
	KeyboardStateTracker* m_keyboardStateTracker;
};

