#include <pch.h>
#include "AIController.h"


AIController::AIController()
{
	reset();
}


AIController::~AIController()
{
}

void AIController::reset(){
	m_state.buttons.a = false;
	m_state.buttons.b = false;
	m_state.buttons.x = false;
	m_state.buttons.y = false;
	m_state.buttons.back = false;
	m_state.buttons.leftShoulder = false;
	m_state.buttons.leftStick = false;
	m_state.buttons.rightShoulder = false;
	m_state.buttons.rightStick = false;
	m_state.buttons.start = false;

	m_state.dpad.down = false;
	m_state.dpad.left = false;
	m_state.dpad.right = false;
	m_state.dpad.up = false;
}

AIController::State AIController::GetState(){
	return m_state;
}