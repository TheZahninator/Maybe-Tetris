#pragma once

#include "Block.h"
#include "Field.h"
#include "ERotation.h"

class Block;

class Tetromino{
public:

	Tetromino(Field& field);
	Tetromino(Field& field, int width, int height);
	Tetromino(Field& field, sf::Vector2i& position, bool* matrix, int width, int height, sf::Color& color);
	~Tetromino();

	void update(KeyboardStateTracker* keyboardTracker);
	void render(sf::RenderWindow* window);
	void render(sf::RenderWindow* window, const sf::Vector2f& screenPosition, float scale = 1.0f);

	//void update(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState);
	//void render(DirectX::SpriteBatch* spriteBatch);
	//void render(DirectX::SpriteBatch* spriteBatch, sf::Vector2i& screenPosition, float scale = 1.0f);

	void setField(Field& field);

	void placeOnField();

	void rotate(ERotation rotation);

	bool setPosition(sf::Vector2i& position);
	bool move(sf::Vector2i& direction);
	void addBlock(Block& block);

	Tetromino* getPart(int* matrix, bool* checked, int width, int height, int x, int y);
	int getBlockCount();

	sf::Vector2i getMatrixSize(){ return sf::Vector2i((float)mMatrixWidth, (float)mMatrixHeight); }
	sf::Vector2i getBlockMostLeft();
	sf::Vector2i getBlockMostRight();

	int* getBaseMatrix(){ return mBaseMatrix.get(); }
	int* getFieldMatrix(){ return mFieldMatrix.get(); }
	int* getOriginalMatrix(){ return m_originalMatrix.get(); }

	bool isLocked(){ return mLocked; }
	bool collidedAtSpawn(){ return mCollisionAtSpawn; }


	UINT getEstimatedTouchingBlocks(){ return m_estimatedTouchingBlocks; }
	UINT getTouchingBlocks(){ return m_touchingBlocks; }
private:
	void setFieldMatrix();

	sf::Vector2f m_positionSreen;
	sf::Vector2i m_positionGrid;
	sf::Vector2i m_estimatedPositionGrid;
	
	std::unique_ptr<int> mBaseMatrix;
	std::unique_ptr<int> mFieldMatrix;
	std::unique_ptr<int> m_originalMatrix;
	unsigned mMatrixWidth, mMatrixHeight;

	bool mLocked;
	bool mCollisionAtSpawn;

	std::vector<std::shared_ptr<Block>> mBlocks;

	float mFrameCounter;
	UINT m_triesToRotate;

	UINT m_estimatedTouchingBlocks;
	UINT m_touchingBlocks;
	void calcTouchingBlocks(UINT& result);

	Field& m_field;
};