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
    m_outputWidth(GAME_WINDOW_WIDTH),
    m_outputHeight(GAME_WINDOW_HEIGHT),
	m_isFullscreen(false)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(std::shared_ptr<sf::RenderWindow> window, int width, int height)
{
	TextureManager::init();

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

	m_field.init(sf::Vector2f(150, 300 - 360 / 2));
	m_field.setScreenSize(sf::Vector2i(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT));
}

void Game::Run()
{	
	m_running = true;

	while (m_running){
		Tick();
	}

	m_window->close();
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
	sf::Event event;

	m_keyboardStateTracker->reset();

	while (m_window->pollEvent(event)){
		switch (event.type)
		{
		case sf::Event::Closed:
			m_running = false;
			break;

		case sf::Event::KeyPressed:
			m_keyboardStateTracker->press(event.key.code);
			break;

		case sf::Event::KeyReleased:
			m_keyboardStateTracker->release(event.key.code);
			break;

		default:
			break;
		}
	}

    float elapsedTime = float(timer.GetElapsedSeconds());

	if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Escape)){
		m_running = false;
	}

	if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::F)){
		switchFullscreen();
	}

	double oldFPS = m_currentFPS;

	if (m_keyboardStateTracker->isKeyDown(sf::Keyboard::Multiply)){
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad2))    m_currentFPS *= 2;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad3))	m_currentFPS *= 3;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad4))	m_currentFPS *= 4;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad5))	m_currentFPS *= 5;
	}
	if (m_keyboardStateTracker->isKeyDown(sf::Keyboard::Divide)){
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad2))	m_currentFPS /= 2;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad3))	m_currentFPS /= 3;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad4))	m_currentFPS /= 4;
		if (m_keyboardStateTracker->isKeyPressed(sf::Keyboard::Numpad5))	m_currentFPS /= 5;
	}

	if (m_currentFPS < 60) {
		m_currentFPS = 60;
	}

	if (m_currentFPS != oldFPS){
		std::cout << "FPS: " << (int)m_currentFPS << std::endl;
		m_timer.SetTargetElapsedSeconds(1.0 / m_currentFPS);
	}

	m_field.Update(m_keyboardStateTracker.get());
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