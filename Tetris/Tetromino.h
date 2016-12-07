#pragma once

#include "Block.h"
#include "Field.h"
#include "ERotation.h"

using namespace DirectX::SimpleMath;

class Block;

class Tetromino{
public:

	Tetromino();
	Tetromino(int width, int height);
	Tetromino(Vector2& position, bool* matrix, int width, int height, Vector4& color);
	~Tetromino();

	void update(DirectX::GamePad::ButtonStateTracker* gamePadTracker, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState);
	void render(DirectX::SpriteBatch* spriteBatch);
	void render(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2& screenPosition, float scale = 1.0f);

	void placeOnField();

	void rotate(ERotation rotation);

	bool setPosition(Vector2& position);
	bool move(Vector2& direction);
	void addBlock(Block& block);

	Tetromino* getPart(int* matrix, bool* checked, int width, int height, int x, int y);
	int getBlockCount();

	Vector2 getMatrixSize(){ return Vector2((float)mMatrixWidth, (float)mMatrixHeight); }
	Vector2 getBlockMostLeft();
	Vector2 getBlockMostRight();

	int* getBaseMatrix(){ return mBaseMatrix.get(); }
	int* getFieldMatrix(){ return mFieldMatrix.get(); }

	bool isLocked(){ return mLocked; }
	bool collidedAtSpawn(){ return mCollisionAtSpawn; }

private:
	void setFieldMatrix();

	Vector2 mPositionSreen;
	Vector2 mPositionGrid;
	
	std::unique_ptr<int> mBaseMatrix;
	std::unique_ptr<int> mFieldMatrix;
	int mMatrixWidth, mMatrixHeight;

	bool mLocked;
	bool mCollisionAtSpawn;

	std::vector<std::shared_ptr<Block>> mBlocks;

	float mFrameCounter;
};