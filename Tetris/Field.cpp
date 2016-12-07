#include "pch.h"
#include "Field.h"

Block** Field::mGrid;
const int Field::mHeight = 20;
const int Field::mWidth = 10;

DirectX::SimpleMath::Vector2 Field::mScreenPosition;
std::vector<std::shared_ptr<TetrominoGroup>> Field::mTetrominoQueue;

float Field::mGravity;
UINT Field::mPoints;
UINT Field::mTotalLinesCleared;

std::vector<std::unique_ptr<ComputerPlayer>>	Field::AIList;
bool											Field::AIMode;
unsigned										Field::AICount;
unsigned										Field::CurrentAI;
unsigned										Field::SurvivingAIs = 3;

void Field::init(DirectX::SimpleMath::Vector2& screenPosition){
	mScreenPosition = screenPosition;
	mGrid = new Block*[mWidth * mHeight];
	memset(mGrid, 0, mWidth * mHeight * sizeof(Block*));

	for (int i = 0; i < QUEUE_SIZE; i++){
		mTetrominoQueue.push_back(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(rand() % NUMBER_OF_MINOS + 1)));
	}

	mGravity = 1.0f;
	mPoints = 0;
	mTotalLinesCleared = 0;

	AICount = 10;
	for (unsigned i = 0; i < AICount; i++){
		AIList.push_back(std::unique_ptr<ComputerPlayer>(new ComputerPlayer()));
	}

	AIMode = false;
}

void Field::restart(){
	if (mGrid)
		delete[] mGrid;

	mTetrominoQueue.clear();

	mGrid = new Block*[mWidth * mHeight];
	memset(mGrid, 0, mWidth * mHeight * sizeof(Block*));

	for (int i = 0; i < QUEUE_SIZE; i++){
		mTetrominoQueue.push_back(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(rand() % NUMBER_OF_MINOS + 1)));
	}

	if (AIMode){
		AIList[CurrentAI]->m_fitness += mPoints;
		CurrentAI++;

		if (CurrentAI >= AICount){
			NewGen();

			CurrentAI = 0;
		}
	}

	mGravity = 1.0f;
	mPoints = 0;
	mTotalLinesCleared = 0;
}

void Field::NewGen(){
	if (AIList.size() < SurvivingAIs)
		SurvivingAIs = AIList.size();

	std::vector<std::unique_ptr<ComputerPlayer>> survived;
	survived.clear();

	while (survived.size() < SurvivingAIs){
		ComputerPlayer* highestFitness = AIList.front().get();
		unsigned x = 0;

		for (unsigned i = 1; i < AIList.size(); i++){
			if (AIList[i]->m_fitness > highestFitness->m_fitness){
				highestFitness = AIList[i].get();
				x = i;
			}
		}

		survived.push_back(std::move(AIList[x]));
		AIList.erase(AIList.begin() + x);
}
	//Clear car list.
	AIList.clear();

	//Write survived cars back into the list.
	for (unsigned i = 0; i < survived.size(); i++){
		AIList.push_back(std::move(survived[i]));
		AIList.back()->m_fitness = 0;
	}

	//Calculate chances for every surviver to recreate.
	float chanceScale = 1.5;

	float totalChance = 0;

	std::vector<float> chances;
	chances.push_back(1);

	totalChance += 1;

	for (unsigned i = 1; i < SurvivingAIs; i++){
		float chance = chances[i - 1] / chanceScale;
		totalChance += chance;
		chances.push_back(chance);
	}

	while (AIList.size() < AICount){
		int num = rand();
		float result = static_cast<float>(ZAHN_MAP(static_cast<double>(num), 0.0, static_cast<double>(RAND_MAX), 0.0, static_cast<double>(totalChance)));

		float cumlutativeChance = totalChance;

		for (unsigned i = 0; i < chances.size(); i++){
			cumlutativeChance -= chances[i];
			if (result >= cumlutativeChance){

				AIList.push_back(std::unique_ptr<ComputerPlayer>(AIList[i]->recreate()));
				break;
			}
		}
	}
	CurrentAI = 0;
}

void Field::Update(DirectX::GamePad::ButtonStateTracker* inputGamePad, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState, DirectX::Mouse::State* mouse, DirectX::Mouse::ButtonStateTracker* mouseTracker){
#ifdef _DEBUG
	DirectX::SimpleMath::Vector2 mousePos(0, 0);
	if(mouse->x > mScreenPosition.x && mouse->x < mScreenPosition.x + mWidth * BLOCK_SIZE &&
		mouse->y > mScreenPosition.y && mouse->y < mScreenPosition.y + mHeight * BLOCK_SIZE){
		int mX = (int)(mouse->x - mScreenPosition.x) / BLOCK_SIZE;
		int mY = (int)(mouse->y - mScreenPosition.y) / BLOCK_SIZE;
		mousePos = DirectX::SimpleMath::Vector2((float)mX, (float)mY);

		if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED){
			if (mGrid[mY * mWidth + mX])
				delete mGrid[mY * mWidth + mX];

			mGrid[mY * mWidth + mX] = new Block(mousePos, DirectX::SimpleMath::Vector4(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 1.0f));
		}
	}
#endif	
	
	//Let the AI controll the Button presses
	if (AIMode){

		AIList[CurrentAI]->update();
		auto state = AIList[CurrentAI]->m_controller->GetState();
		inputGamePad->Update(state);

	}

	//Updatet den aktiven Tetromino.
	mTetrominoQueue[0]->update(inputGamePad, keyboardTracker, keyboardState);

	//Wenn der aktive Tetromino gesetzt wurde, wird er gelöscht und die Tetrominos in der Schlange rücken nach. Ein neuer reiht sich hinten ein.
	if (mTetrominoQueue[0]->shouldDestroy()){
		mTetrominoQueue.erase(mTetrominoQueue.begin());
		mTetrominoQueue.push_back(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(rand() % NUMBER_OF_MINOS + 1)));
	}
}

void Field::Render(DirectX::SpriteBatch* spritebatch){
	//Zeichnet den Hintergrund
	TextureManager::getTexture(TEX_BACKGROUND_1)->draw(spritebatch, DirectX::SimpleMath::Vector2(0.0f, 0.0f));

	Texture* borderTexture = TextureManager::getTexture(TEX_BACKGROUND_FIELD_BORDER);
	DirectX::SimpleMath::Vector2 borderOffset;
	borderOffset.x = (mWidth * BLOCK_SIZE - borderTexture->getWidth()) / 2.0f;
	borderOffset.y = (mHeight * BLOCK_SIZE - borderTexture->getHeight()) / 2.0f;

	borderTexture->draw(spritebatch, mScreenPosition + borderOffset);

	//Zeichnet transparente Blöcke als Spielfeldhintergrund.
	for (int y = 0; y < mHeight; y++){
		for (int x = 0; x < mWidth; x++){
			DirectX::SimpleMath::Vector2 pos = mScreenPosition + DirectX::SimpleMath::Vector2((float)x, (float)y) * BLOCK_SIZE;
			TextureManager::getTexture(TEX_BACKGROUND_FIELD_TILE)->draw(spritebatch, pos, BLOCK_COLOR_WHITE * COLOR_ALPHA_25);
		}
	}

	//Zeichnet die auf dem Spielfeld platzierten Blöcke.
	for (int y = 0; y < mHeight; y++){
		for (int x = 0; x < mWidth; x++){
			if (mGrid[y * mWidth + x]){
				mGrid[y * mWidth + x]->render(spritebatch);
			}
		}
	}

	//Zeichnet den aktiven Tetromino
	mTetrominoQueue[0]->render(spritebatch);

	//Zeichnet die eingereiten tetrominos neben das Spielfeld.
	for (UINT i = 1; i < mTetrominoQueue.size(); i++){
		mTetrominoQueue[i]->render(spritebatch, mScreenPosition + DirectX::SimpleMath::Vector2(float((mWidth + 2) * BLOCK_SIZE), float((i - 1) * BLOCK_SIZE * 3.5f)), 0.75f);
	}
}

void Field::checkForLineClear(){
	int linesCleared = 0;

	for (int y = 0; y < mHeight; y++){

		int blocksInLine = 0;

		for (int x = 0; x < mWidth; x++){
			if (mGrid[y * mWidth + x]){
				blocksInLine++;
			}

			if (blocksInLine >= 10){
				clearLine(y, linesCleared);
			}
		}
	}

	for (int y = 0; y < mHeight; y++){
		for (int x = 0; x < mWidth; x++){
			if (mGrid[y * mWidth + x])
				mGrid[y * mWidth + x]->setPosition(DirectX::SimpleMath::Vector2((float)x, (float)y));
		}
	}

	mPoints += 1000 * linesCleared * linesCleared;

	mTotalLinesCleared += linesCleared;
	mGravity = mTotalLinesCleared / 10.0f + 1.0f;
}

void Field::clearLine(int y, int& linesCleared){
	for (int x = 0; x < mWidth; x++){
		delete mGrid[y * mWidth + x];
		mGrid[y * mWidth + x] = nullptr;
	}
	linesCleared++;

	for (int j = y; j > 0; j--){
		for (int i = 0; i < mWidth; i++){
			mGrid[j * mWidth + i] = mGrid[(j - 1) * mWidth + i];
		}
	}
	for (int i = 0; i < mWidth; i++)
		mGrid[i] = nullptr;
}

void Field::addBlock(Block* block){
	int x = (int)block->getPosition().x;
	int y = (int)block->getPosition().y;

	if (mGrid[y * mWidth + x])
		delete mGrid[y * mWidth + x];

	mGrid[y * mWidth + x] = block;
}


