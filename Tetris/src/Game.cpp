//
// Game.cpp
//

#include <pch.h>
#include "Game.h"
#include <CommonStates.h>
#include <time.h>

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() :
    m_window(0),
    m_outputWidth(GAME_WINDOW_WIDTH),
    m_outputHeight(GAME_WINDOW_HEIGHT),
	m_isFullscreen(false)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(std::shared_ptr<sf::RenderWindow> window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);
	m_running = true;

	m_currentFPS = GAME_DEFAULT_FPS;

    m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / m_currentFPS);

	srand((unsigned)time(nullptr));

	m_noRender = false;

	m_keyboardStateTracker.reset(new KeyboardStateTracker());

	m_field.init(sf::Vector2f(0, 0));
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

	if (!m_noRender)
	   Render();

	std::this_thread::sleep_for(std::chrono::microseconds(1));
}

// Updates the world.
void Game::Update(StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

	/*
	auto state = mGamePad->GetState(0);
	if (state.IsConnected()){
		mGamePadTracker->Update(state);

	}
	else{
		AI::m_controller->reset();
		state = AI::m_controller->GetState();
		mGamePadTracker->Update(state);
	}
    
	elapsedTime;
	*/
	m_field.Update(m_keyboardStateTracker.get());

	if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Escape)){
		PostQuitMessage(0);
	}

	if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::F)){
		switchFullscreen();
	}

	double oldFPS = m_currentFPS;

	if (m_keyboardStateTracker->isKeyDown(sf::Keyboard::Multiply)){
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num2))	m_currentFPS *= 2;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num3))	m_currentFPS *= 3;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num4))	m_currentFPS *= 4;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num5))	m_currentFPS *= 5;
	}
	if (m_keyboardStateTracker->isKeyDown(sf::Keyboard::Divide)){
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num2))	m_currentFPS /= 2;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num3))	m_currentFPS /= 3;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num4))	m_currentFPS /= 4;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Num5))	m_currentFPS /= 5;
	}

	if (m_currentFPS < 60) {
		m_currentFPS = 60;
	}

	//if (mKeyboard->RightControl && mKeyboardTracker->IsKeyPressed(mKeyboard->R)){
	//	m_noRender = !m_noRender;
	//}

	if (m_currentFPS != oldFPS){
		m_timer.SetTargetElapsedSeconds(1.0 / m_currentFPS);
	}
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    m_window->clear();

	m_field.Render(m_window.get());

 //   // TODO: Add your rendering code here.
	//CommonStates states(m_d3dDevice.Get());

	//mSpriteBatch->Begin(SpriteSortMode_Deferred, nullptr, states.PointClamp(), nullptr, nullptr, nullptr, SimpleMath::Matrix::CreateScale(1.0f));
	//
	//Field::Render(mSpriteBatch.get(), m_spriteFont.get());

	//mSpriteBatch->End();

    m_window->display();
}

bool Game::isRunning(){
	return m_running;
}

void Game::switchFullscreen(){
	if (!m_isFullscreen){
		m_window.reset(new sf::RenderWindow(sf::VideoMode(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), GAME_TITLE, sf::Style::Fullscreen));
		m_isFullscreen = true;
	}
	else{
		m_window.reset(new sf::RenderWindow(sf::VideoMode(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT), GAME_TITLE));
		m_isFullscreen = false;
	}
}

KeyboardStateTracker* Game::getKeyboardStateTracker(){
	return m_keyboardStateTracker.get();
}