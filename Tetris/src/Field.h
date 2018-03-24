#pragma once

#include <vector>

#include "TetrominoGroup.h"
#include "AI.h"
#include "BagContainer.h"

//#define PUGIXML_WCHAR_MODE
#include "pugixml.hpp"

#define QUEUE_SIZE 7

class Block;
class TetrominoGroup;

class Field{
public:
	static void init(DirectX::SimpleMath::Vector2& screenPosition);
	static void restart();

	static void Update(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState, DirectX::Mouse::State* mouse, DirectX::Mouse::ButtonStateTracker* mouseTracker);
	static void Render(DirectX::SpriteBatch* spritebatch, DirectX::SpriteFont* spriteFont);

	static void checkForLineClear();

	static void addBlock(std::shared_ptr<Block> block);

	static std::shared_ptr<Block>* getGrid(){ return mGrid; }
	static bool getGrid(int x, int y){
		if (x >= 0 && x < mWidth && y >= 0 && y < mHeight){
			return mGrid[y * mWidth + x] != nullptr;
		}
		else{
			return true;
		}
	}
	static DirectX::SimpleMath::Vector2& getScreenPosition(){ return mScreenPosition; }

	static UINT getWidth(){ return mWidth; }
	static UINT getHeight(){ return mHeight; }

	static float getGravity(){ return mGravity; }

	static UINT getPoints(){ return mPoints; }
	static TetrominoGroup* getCurrentTetromino(){ return mTetrominoQueue[0].get(); }
	static std::vector<std::shared_ptr<TetrominoGroup>>& getQueue(){ return mTetrominoQueue; }

	static std::vector<std::unique_ptr<AI>> AIList;
	static bool AIMode;
	static unsigned AICount;
	static unsigned CurrentAI;
	static unsigned SurvivingAIs;
	static unsigned CurrentGen;

	static void NewGen();

	static void saveGenomes();
	static void loadGenomes();

private:
	static void clearLine(int y, int& linesCleared);

	static std::vector<std::shared_ptr<TetrominoGroup>> mTetrominoQueue; //Die warteschlange für die kommenden Tetrominos.
	static BagContainer<std::shared_ptr<TetrominoGroup>> m_bag;
	static UINT m_copiesInBag;

	static const UINT mWidth;	//Spielfeldbreite
	static const UINT mHeight;	//Spielfeldhöhe
	static std::shared_ptr<Block>* mGrid;	//Auf dem Spielfeld festgesetzte Blöcke

	static DirectX::SimpleMath::Vector2 mScreenPosition;	//Spielfeldposition auf dem Bildschirm

	static float mGravity; //Anzahl der Felder, die ein Tetromino pro Sekunde fällt.
	static UINT mPoints;	//Punktestand
	static UINT m_Highscore;
	static UINT m_HighscoreAI;

	static UINT mTotalLinesCleared; //Anzahl der Reihen die insgesamt aufgelöst wurden. Bestimmt den Wert von mGravity.
	static UINT m_totalLinesHighscore;
	static UINT m_totalLinesHighscoreAI;

	static UINT m_framesSinceLastTetromino;
	static UINT m_AILearningFrameCounter;

	static int m_lastTetrominoType;

	static void fillBag();
	static void drawFromBag();

	static void switchAIMode();

	static bool pressedButtons[6]; //Up, Down, Left, Right, CW, CCW
};