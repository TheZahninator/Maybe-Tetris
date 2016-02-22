#pragma once

#include "Tetromino.h"
#include "TetrominoMatrices.h"
#include "Field.h"
#include "ERotation.h"

class Tetromino;

class TetrominoGroup{
public:
	TetrominoGroup(int type);
	~TetrominoGroup();

	void update(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState);
	void render(DirectX::SpriteBatch* spriteBatch);
	void render(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2& screenPosition, float scale = 1.0f);

	bool move(DirectX::SimpleMath::Vector2& direction);
	void rotate(ERotation rotation);
	bool setPosition(DirectX::SimpleMath::Vector2& position);
	DirectX::SimpleMath::Vector2& getPosition(){ return mPositionGrid; }

	bool shouldDestroy(){ return mToBeDestroyed; }

private:
	void split(Tetromino* tetromino);
	bool mSplitted;
	bool mToBeDestroyed;
	
	std::vector<Tetromino*> mTetrominos;
	DirectX::SimpleMath::Vector2 mPositionGrid;
};

