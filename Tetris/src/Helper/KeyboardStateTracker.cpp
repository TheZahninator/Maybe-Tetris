#include "pch.h"
#include "KeyboardStateTracker.h"


KeyboardStateTracker::KeyboardStateTracker()
{
	for (unsigned i = 0; i < BufferSize; i++){
		m_keysDown[i] = false;
		m_keysPressed[i] = false;
		m_keysReleased[i] = false;
	}
}


KeyboardStateTracker::~KeyboardStateTracker()
{
}

void KeyboardStateTracker::press(int keyCode){
	if (keyCode < 0){
		return;
	}

	if (!m_keysDown[keyCode]){
		m_keysPressed[keyCode] = true;
	}

	m_keysDown[keyCode] = true;
}

void KeyboardStateTracker::release(int keyCode){
	if (keyCode < 0){
		return;
	}

	if (m_keysDown[keyCode]){
		m_keysReleased[keyCode] = true;
	}

	m_keysDown[keyCode] = false;
}

void KeyboardStateTracker::reset(){
	for (unsigned i = 0; i < BufferSize; i++){
		m_keysPressed[i] = false;
		m_keysReleased[i] = false;
	}
}

bool KeyboardStateTracker::isKeyPressed(int keyCode){
	if (keyCode < 0){
		throw std::invalid_argument("keyCode has to be positive");
	}

	return m_keysPressed[keyCode];
}

bool KeyboardStateTracker::isKeyReleased(int keyCode){
	if (keyCode < 0){
		throw std::invalid_argument("keyCode has to be positive");
	}

	return m_keysReleased[keyCode];
}

bool KeyboardStateTracker::isKeyDown(int keyCode){
	if (keyCode < 0){
		throw std::invalid_argument("keyCode has to be positive");
	}

	return m_keysDown[keyCode];
}