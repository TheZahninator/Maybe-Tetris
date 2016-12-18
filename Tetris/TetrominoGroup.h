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

	int* getMatrix();
	DirectX::SimpleMath::Vector2 getMatrixSize();
	int getRotation(){ return m_rotation; }
	int getType(){ return m_type; }

private:
	void split(Tetromino* tetromino);
	bool mSplitted;
	bool mToBeDestroyed;
	
	std::vector<std::shared_ptr<Tetromino>> mTetrominos;
	DirectX::SimpleMath::Vector2 mPositionGrid;

	int m_rotation;
	int m_type;
};

