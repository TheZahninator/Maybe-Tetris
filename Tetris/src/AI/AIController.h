#pragma once
class AIController
{
public:
	AIController(KeyboardStateTracker* keyboardStateTracker);
	~AIController();

	void reset();

	void pressCCW();
	void releaseCCW();

	void pressCW();
	void releaseCW();
	
	void pressLeft();
	void releaseLeft();
	
	void pressRight();
	void releaseRight();
	
	void pressUp();
	void releaseUp();
	
	void pressDown();
	void releaseDown();

private:
	KeyboardStateTracker* m_keyboardStateTracker;
};

