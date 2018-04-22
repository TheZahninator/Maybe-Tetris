#pragma once

#include <vector>

#include "TetrominoGroup.h"
#include "../AI/AI.h"
#include "../Helper/BagContainer.h"

#include "../Helper/pugixml.hpp"

#define QUEUE_SIZE 8

class Block;
class TetrominoGroup;
class AI;

struct ReplayData{
	unsigned frameNumber;
	bool pressedButtons[6];
	bool newTetromino;
	byte tetrominoQueue[QUEUE_SIZE];

	std::vector<double> inputData;
};

class Field{
public:
	Field();

	void init(sf::Vector2f& screenPosition);
	void restart();

	void Update(KeyboardStateTracker* keyboardTracker);
	void Render(sf::RenderWindow* window);

	void checkForLineClear();

	void addBlock(std::shared_ptr<Block> block);

	void nextTetromino();

	std::shared_ptr<Block>* getGrid(){ return mGrid; }
	bool getGrid(unsigned x, unsigned y){
		if (x >= 0 && x < mWidth && y >= 0 && y < mHeight){
			return mGrid[y * mWidth + x] != nullptr;
		}
		else{
			return true;
		}
	}

	void setScreenSize(const sf::Vector2i& size){ m_screenSize = size; }
	sf::Vector2f& getScreenPosition(){ return mScreenPosition; }

	UINT getWidth(){ return mWidth; }
	UINT getHeight(){ return mHeight; }

	float getGravity(){ return m_gravity; }

	UINT getPoints(){ return m_score; }
	TetrominoGroup* getCurrentTetromino(){ return mTetrominoQueue[0].get(); }
	std::vector<std::shared_ptr<TetrominoGroup>>& getQueue(){ return mTetrominoQueue; }

	std::vector<std::unique_ptr<AI>> AIList;
	bool AIMode;
	unsigned AICount;
	unsigned CurrentAI;
	unsigned SurvivingAIs;
	unsigned CurrentGen;

	void NewGen();

	void saveBest();
	void loadBest();

	void saveReplay();

private:
	void clearLine(int y, int& linesCleared);

	std::vector<std::shared_ptr<TetrominoGroup>> mTetrominoQueue;
	BagContainer<std::shared_ptr<TetrominoGroup>> m_bag;
	UINT m_copiesInBag;

	const UINT mWidth;	
	const UINT mHeight;
	std::shared_ptr<Block>* mGrid;
	sf::Vector2i m_screenSize;

	sf::Vector2f mScreenPosition;	

	float m_gravity; 
	UINT m_score;	
	UINT m_Highscore;
	UINT m_HighscoreAI;

	UINT m_totalLinesCleared; 
	UINT m_totalLinesHighscore;
	UINT m_totalLinesHighscoreAI;

	UINT m_framesSinceLastTetromino;
	UINT m_AILearningFrameCounter;

	int m_lastTetrominoType;

	bool m_trainingMode;

	void fillBag();
	void drawFromBag();

	void switchAIMode();
	void switchTrainingMode();

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

private:
	std::vector<ReplayData> m_replayData;

	bool m_recordMode;

	bool m_spawnedNewTetromino;
	unsigned m_recordModeFrameCount;

	void switchRecordMode();
};