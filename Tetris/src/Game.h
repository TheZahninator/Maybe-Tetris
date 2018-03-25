//
// Game.h
//

#pragma once

#define GAME_TITLE L"Türme-Bau-Simulator 2016"
#define DEFAULT_FPS 60.0f

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
    void Initialize(HWND window, int width, int height);

    // Basic game loop
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

private:

    void Update(DX::StepTimer const& timer);

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

		
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//EIGNE VARIABLEN
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	std::unique_ptr<SpriteBatch> mSpriteBatch;
	std::unique_ptr<SpriteFont> m_spriteFont;
	std::shared_ptr<GamePad> mGamePad;
	std::unique_ptr<GamePad::ButtonStateTracker> mGamePadTracker;
	std::unique_ptr<Keyboard> mKeyboard;
	std::unique_ptr<Keyboard::KeyboardStateTracker> mKeyboardTracker;
	std::unique_ptr<Mouse> mMouse;
	std::unique_ptr<Mouse::ButtonStateTracker> mMouseButtonTracker;

	bool m_running;

	bool m_noRender;

	WINDOWPLACEMENT m_wpPrev;
	void switchFullscreen();

	double m_currentFPS;
};