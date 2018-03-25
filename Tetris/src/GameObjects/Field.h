#pragma once

#include <vector>

#include "TetrominoGroup.h"
#include "../AI/AI.h"
#include "../Helper/BagContainer.h"

//#define PUGIXML_WCHAR_MODE
#include "../Helper/pugixml.hpp"

#define QUEUE_SIZE 7

class Block;
class TetrominoGroup;

class Field{
public:
	void init(sf::Vector2f& screenPosition);
	void restart();

	void Update(KeyboardStateTracker* keyboardTracker);
	void Render(sf::RenderWindow* window);

	//static void Update(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState, DirectX::Mouse::State* mouse, DirectX::Mouse::ButtonStateTracker* mouseTracker);
	//static void Render(DirectX::SpriteBatch* spritebatch, DirectX::SpriteFont* spriteFont);

	void checkForLineClear();

	void addBlock(std::shared_ptr<Block> block);

	std::shared_ptr<Block>* getGrid(){ return mGrid; }
	bool getGrid(unsigned x, unsigned y){
		if (x >= 0 && x < mWidth && y >= 0 && y < mHeight){
			return mGrid[y * mWidth + x] != nullptr;
		}
		else{
			return true;
		}
	}
	sf::Vector2f& getScreenPosition(){ return mScreenPosition; }

	UINT getWidth(){ return mWidth; }
	UINT getHeight(){ return mHeight; }

	float getGravity(){ return mGravity; }

	UINT getPoints(){ return mPoints; }
	TetrominoGroup* getCurrentTetromino(){ return mTetrominoQueue[0].get(); }
	std::vector<std::shared_ptr<TetrominoGroup>>& getQueue(){ return mTetrominoQueue; }

	std::vector<std::unique_ptr<AI>> AIList;
	bool AIMode;
	unsigned AICount;
	unsigned CurrentAI;
	unsigned SurvivingAIs;
	unsigned CurrentGen;

	void NewGen();

	void saveGenomes();
	void loadGenomes();

private:
	void clearLine(int y, int& linesCleared);

	std::vector<std::shared_ptr<TetrominoGroup>> mTetrominoQueue; //Die warteschlange für die kommenden Tetrominos.
	BagContainer<std::shared_ptr<TetrominoGroup>> m_bag;
	UINT m_copiesInBag;

	const UINT mWidth;	//Spielfeldbreite
	const UINT mHeight;	//Spielfeldhöhe
	std::shared_ptr<Block>* mGrid;	//Auf dem Spielfeld festgesetzte Blöcke

	sf::Vector2f mScreenPosition;	//Spielfeldposition auf dem Bildschirm

	float mGravity; //Anzahl der Felder, die ein Tetromino pro Sekunde fällt.
	UINT mPoints;	//Punktestand
	UINT m_Highscore;
	UINT m_HighscoreAI;

	UINT mTotalLinesCleared; //Anzahl der Reihen die insgesamt aufgelöst wurden. Bestimmt den Wert von mGravity.
	UINT m_totalLinesHighscore;
	UINT m_totalLinesHighscoreAI;

	UINT m_framesSinceLastTetromino;
	UINT m_AILearningFrameCounter;

	int m_lastTetrominoType;

	void fillBag();
	void drawFromBag();

	void switchAIMode();

	bool pressedButtons[6]; //Up, Down, Left, Right, CW, CCW

	sf::Sprite m_backgroundSprite;
	sf::Sprite m_backgroundBorderSprite;
	sf::Sprite m_backgroundOverlaySprite;
	sf::Sprite m_backgroudFieldTileSprite;

	sf::Sprite m_keyUpSprite;
	sf::Sprite m_keyDownSprite;
	sf::Sprite m_keyLeftSprite;
	sf::Sprite m_keyRightSprite;
	sf::Sprite m_keyClockwiseSprite;
	sf::Sprite m_keyCounterClockwiseSprite;
};