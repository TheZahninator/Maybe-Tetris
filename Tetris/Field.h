#pragma once

#include <vector>

#include "TetrominoGroup.h"

#define QUEUE_SIZE 7

class Block;
class TetrominoGroup;

class Field{
public:
	static void init(DirectX::SimpleMath::Vector2& screenPosition);

	static void Update(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState, DirectX::Mouse::State* mouse, DirectX::Mouse::ButtonStateTracker* mouseTracker);
	static void Render(DirectX::SpriteBatch* spritebatch);

	static void checkForLineClear();

	static void addBlock(Block* block);

	static Block** getGrid(){ return mGrid; }
	static DirectX::SimpleMath::Vector2& getScreenPosition(){ return mScreenPosition; }

	static int getWidth(){ return mWidth; }
	static int getHeight(){ return mHeight; }

	static float getGravity(){ return mGravity; }

private:
	static void clearLine(int y, int& linesCleared);
	
	static std::vector<TetrominoGroup*> mTetrominoQueue; //Die warteschlange f�r die kommenden Tetrominos.

	static const int mWidth;	//Spielfeldbreite
	static const int mHeight;	//Spielfeldh�he
	static Block** mGrid;	//Auf dem Spielfeld festgesetzte Bl�cke

	static DirectX::SimpleMath::Vector2 mScreenPosition;	//Spielfeldposition auf dem Bildschirm

	static float mGravity; //Anzahl der Felder, die ein Tetromino pro Sekunde f�llt.
	static UINT mPoints;	//Punktestand
	static UINT mTotalLinesCleared; //Anzahl der Reihen die insgesamt aufgel�st wurden. Bestimmt den Wert von mGravity.
};