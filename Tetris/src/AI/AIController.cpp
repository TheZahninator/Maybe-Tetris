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


void AIController::rotateCCW(){
	m_keyboardStateTracker->press(sf::Keyboard::A);
	m_keyboardStateTracker->release(sf::Keyboard::A);
}

void AIController::rotateCW(){
	m_keyboardStateTracker->press(sf::Keyboard::D);
	m_keyboardStateTracker->release(sf::Keyboard::D);
}

void AIController::moveUp(){
	m_keyboardStateTracker->press(sf::Keyboard::Up);
	m_keyboardStateTracker->release(sf::Keyboard::Up);
}

void AIController::moveDown(){
	m_keyboardStateTracker->press(sf::Keyboard::Down);
	m_keyboardStateTracker->release(sf::Keyboard::Down);
}

void AIController::moveLeft(){
	m_keyboardStateTracker->press(sf::Keyboard::Left);
	m_keyboardStateTracker->release(sf::Keyboard::Left);
}

void AIController::moveRight(){
	m_keyboardStateTracker->press(sf::Keyboard::Right);
	m_keyboardStateTracker->release(sf::Keyboard::Right);
}