//
// Game.h
//

#pragma once

#define GAME_TITLE L"Türme-Bau-Simulator 2016"
#define GAME_WINDOW_WIDTH 800
#define GAME_WINDOW_HEIGHT 600
#define GAME_DEFAULT_FPS 60.0f

#include "Helper/StepTimer.h"
#include "GameObjects/TetrominoGroup.h"
#include "AI/AI.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(std::shared_ptr<sf::RenderWindow> window, int width, int height);
	
    // Basic game loop
	void Run();
    void Tick();
    void Render();

    // Rendering helpers
    void Clear();
    void Present();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properites
    void GetDefaultSize( int& width, int& height ) const;

	bool isRunning();

	KeyboardStateTracker* getKeyboardStateTracker();

private:

    void Update(StepTimer const& timer);

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    std::shared_ptr<sf::RenderWindow> m_window;
    int m_outputWidth;
    int m_outputHeight;

    // Rendering loop timer.
    StepTimer m_timer;

	std::unique_ptr<KeyboardStateTracker> m_keyboardStateTracker;

	bool m_running;

	bool m_noRender;
	bool m_isFullscreen;

	void switchFullscreen();

	double m_currentFPS;

	Field m_field;
};