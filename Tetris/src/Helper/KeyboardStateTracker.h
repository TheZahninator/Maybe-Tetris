#pragma once

#include <stdexcept>

class KeyboardStateTracker
{
public:
	const static unsigned BufferSize = 1024;

	KeyboardStateTracker();
	~KeyboardStateTracker();

	void press(int keyCode);
	void release(int keyCode);

	void update();

	bool isKeyPressed(int keyCode);
	bool isKeyReleased(int keyCode);

	bool isKeyDown(int keyCode);

private:
	bool m_keysDown[BufferSize];
	bool m_keysPressed[BufferSize];
	bool m_keysReleased[BufferSize];
};

