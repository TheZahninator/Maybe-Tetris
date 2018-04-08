#include <pch.h>
#include "AIController.h"


AIController::AIController(KeyboardStateTracker* keyboardStateTracker) : m_keyboardStateTracker(keyboardStateTracker)
{
}


AIController::~AIController()
{
}

void AIController::reset(){
	m_keyboardStateTracker->reset();
}


void AIController::pressCCW(){
	m_keyboardStateTracker->press(sf::Keyboard::A);
}

void AIController::releaseCCW(){
	m_keyboardStateTracker->release(sf::Keyboard::A);
}

void AIController::pressCW(){
	m_keyboardStateTracker->press(sf::Keyboard::D);
}

void AIController::releaseCW(){
	m_keyboardStateTracker->release(sf::Keyboard::D);
}

void AIController::pressUp(){
	m_keyboardStateTracker->press(sf::Keyboard::Up);
}

void AIController::releaseUp(){
	m_keyboardStateTracker->release(sf::Keyboard::Up);
}

void AIController::pressDown(){
	m_keyboardStateTracker->press(sf::Keyboard::Down);
}

void AIController::releaseDown(){
	m_keyboardStateTracker->release(sf::Keyboard::Down);
}

void AIController::pressLeft(){
	m_keyboardStateTracker->press(sf::Keyboard::Left);
}

void AIController::releaseLeft(){
	m_keyboardStateTracker->release(sf::Keyboard::Left);
}

void AIController::pressRight(){
	m_keyboardStateTracker->press(sf::Keyboard::Right);
}

void AIController::releaseRight(){
	m_keyboardStateTracker->release(sf::Keyboard::Right);
}