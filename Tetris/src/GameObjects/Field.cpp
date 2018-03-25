#include <pch.h>
#include "Field.h"


Field::Field():
mWidth(10), mHeight(20), m_copiesInBag(2), m_framesSinceLastTetromino(0), m_AILearningFrameCounter(0), m_lastTetrominoType(0)
{
	AICount = 25;
	CurrentAI = 0;
	SurvivingAIs = 2;
	CurrentGen = 0;
}

void Field::fillBag(){

	for (unsigned i = 0; i < m_copiesInBag; i++){
		for (int j = 0; j < NUMBER_OF_MINOS; j++){
			m_bag.add(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(j)));
		}
	}
}

void Field::drawFromBag(){
	std::shared_ptr<TetrominoGroup> group;
	
	do{
		if(!mTetrominoQueue.empty())
			m_lastTetrominoType = mTetrominoQueue.back()->getType();
		group = m_bag.draw();
		
		if (m_bag.isEmpty())
			fillBag();
	} while (group->getType() == m_lastTetrominoType);

	mTetrominoQueue.push_back(group);

}

void Field::init(sf::Vector2i& screenPosition){
	m_backgroundSprite = sf::Sprite(TextureManager::getTexture(TEX_BACKGROUND_1));
	m_backgroundBorderSprite = sf::Sprite(TextureManager::getTexture(TEX_BACKGROUND_FIELD_BORDER));
	m_backgroundOverlaySprite = sf::Sprite(TextureManager::getTexture(TEX_BACKGROUND_OVERLAY));
	m_backgroudFieldTileSprite = sf::Sprite(TextureManager::getTexture(TEX_BACKGROUND_FIELD_TILE));

	m_keyUpSprite = sf::Sprite(TextureManager::getTexture(TEX_KEY_OVERLAY));
	m_keyDownSprite = sf::Sprite(TextureManager::getTexture(TEX_KEY_OVERLAY));
	m_keyLeftSprite = sf::Sprite(TextureManager::getTexture(TEX_KEY_OVERLAY));
	m_keyRightSprite = sf::Sprite(TextureManager::getTexture(TEX_KEY_OVERLAY));
	m_keyCounterClockwiseSprite = sf::Sprite(TextureManager::getTexture(TEX_KEY_OVERLAY));
	m_keyClockwiseSprite = sf::Sprite(TextureManager::getTexture(TEX_KEY_OVERLAY));



	sf::Vector2f keyOverlayOffset(425, 375);

	m_keyCounterClockwiseSprite.setPosition(keyOverlayOffset + sf::Vector2f(0, 0));
	m_keyClockwiseSprite.setPosition(keyOverlayOffset + sf::Vector2f(128, 0));
	m_keyUpSprite.setPosition(keyOverlayOffset + sf::Vector2f(64, 0));
	m_keyDownSprite.setPosition(keyOverlayOffset + sf::Vector2f(64, 64));
	m_keyLeftSprite.setPosition(keyOverlayOffset + sf::Vector2f(0, 64));
	m_keyRightSprite.setPosition(keyOverlayOffset + sf::Vector2f(128, 64));

	mScreenPosition = screenPosition;
	mGrid = new std::shared_ptr<Block>[mWidth * mHeight];
	//memset(mGrid, 0, mWidth * mHeight * sizeof(Block*));
	m_bag.clear();

	fillBag();

	for (int i = 0; i < QUEUE_SIZE; i++){
		//mTetrominoQueue.push_back(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(rand() % NUMBER_OF_MINOS + 1)));
		drawFromBag();
	}

	mGravity = 1.0f;
	
	mPoints = 0;
	m_Highscore = 0;
	m_HighscoreAI = 0;

	mTotalLinesCleared = 0;
	m_totalLinesHighscore = 0;
	m_totalLinesHighscoreAI = 0;

	for (unsigned i = 0; i < AICount; i++){
		AIList.push_back(std::unique_ptr<AI>(new AI()));
	}

	AIMode = false;

	m_framesSinceLastTetromino = 0;

	for (unsigned i = 0; i < 6; i++)
		pressedButtons[i] = false;

	restart();
}

void Field::restart(){
	if (mGrid)
		delete[] mGrid;

	mTetrominoQueue.clear();

	mGrid = new std::shared_ptr<Block>[mWidth * mHeight];
	//memset(mGrid, 0, mWidth * mHeight * sizeof(Block*));

	m_bag.clear();

	fillBag();

	for (int i = 0; i < QUEUE_SIZE; i++){
		//mTetrominoQueue.push_back(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(rand() % NUMBER_OF_MINOS + 1)));
		drawFromBag();
	}

	if (AIMode){
		AIList[CurrentAI]->updateFitness(mPoints);
		CurrentAI++;

		if (CurrentAI >= AICount){
			NewGen();

			CurrentAI = 0;
		}


		if (mPoints > m_HighscoreAI)
			m_HighscoreAI = mPoints;

		if (mTotalLinesCleared > m_totalLinesHighscoreAI)
			m_totalLinesHighscoreAI = mTotalLinesCleared;
	}
	else{
		if (mPoints > m_Highscore)
			m_Highscore = mPoints;

		if (mTotalLinesCleared > m_totalLinesHighscore)
			m_totalLinesHighscore = mTotalLinesCleared;
	}

	mGravity = 1.0f;
	mPoints = 0;
	mTotalLinesCleared = 0;
	m_framesSinceLastTetromino = 0;

	m_AILearningFrameCounter = 0;
	//if (!AIMode){
	//	AIMode = true;
	//}
}

void Field::switchAIMode(){
	restart();
	AIMode = !AIMode;
	CurrentAI = 0;
}

void Field::NewGen(){
	if (AIList.size() < SurvivingAIs)
		SurvivingAIs = AIList.size();

	std::vector<std::unique_ptr<AI>> survived;
	survived.clear();

	while (survived.size() < SurvivingAIs){
		AI* highestFitness = AIList.front().get();
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
		AIList.back()->updateFitness(0);
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

				AIList.push_back(std::unique_ptr<AI>(AIList[0]->recreate(AIList[1].get())));
				break;
			}
		}
	}
	CurrentAI = 0;
	CurrentGen++;
}



void Field::saveGenomes(){
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("root");
	pugi::xml_attribute atr = root.append_attribute("popSize");
	atr.set_value(AICount);
	atr = root.append_attribute("highscore");
	atr.set_value(m_HighscoreAI);
	atr = root.append_attribute("lines");
	atr.set_value(m_totalLinesHighscoreAI);
	atr = root.append_attribute("generation");
	atr.set_value(CurrentGen);
	atr = root.append_attribute("memory");
	atr.set_value(AI::MemorySize);

	for (unsigned i = 0; i < AICount; i++){
		pugi::xml_node ai = root.append_child("AI");

		for (unsigned j = 0; j < AIList[i]->getNet()->getLayerCount(); j++){
			ZahnAI::Layer& layer = AIList[i]->getNet()->getLayer(j);
			pugi::xml_node layerNode = ai.append_child("Layer");

			for (unsigned k = 0; k < layer.size(); k++){
				ZahnAI::Neuron& neuron = layer[k];
				pugi::xml_node neuronNode = layerNode.append_child("neuron");

				for (unsigned l = 0; l < neuron.getOutputWeights().size(); l++){
					double weight = neuron.getOutputWeights()[l].weight;
					pugi::xml_attribute weightAtr = neuronNode.append_attribute("weight");
					weightAtr.set_value(weight);
				}
			}
		}
	}

	doc.save_file("res/save.xml");
}

void Field::loadGenomes(){
	pugi::xml_document doc;
	doc.load_file("res/save.xml");

	pugi::xml_node root = doc.child("root");
	pugi::xml_attribute atr = root.attribute("popSize");
	AICount = atr.as_uint();
	atr = root.attribute("highscore");
	if (atr)
		m_HighscoreAI = atr.as_uint();
	atr = root.attribute("lines");
	if (atr)
		m_totalLinesHighscoreAI = atr.as_uint();
	atr = root.attribute("generation");
	if (atr)
		CurrentGen = atr.as_uint();
	atr = root.attribute("memory");
	if (atr)
		AI::MemorySize = atr.as_uint();

	AIList.clear();

	for(auto ai : root.children("AI")){
		AIList.push_back(std::unique_ptr<AI>(new AI()));

		ZahnAI::NeuralNet* net = AIList.back()->getNet().get();

		int i = 0;
		for(auto layer : ai.children("Layer")){

			int j = 0;
			for (auto neuron : layer.children("neuron")){

				int k = 0;
				for (auto weight : neuron.attributes()){

					net->getLayer(i)[j].getOutputWeights()[k].weight = weight.as_double();

					k++;
				}

				j++;
			}

			i++;
		}
	}

	CurrentAI = 0;
	restart();
	CurrentAI = 0;
}

void Field::Update(KeyboardStateTracker* keyboardTracker){
	/*
#ifdef _DEBUG
	sf::Vector2i mousePos(0, 0);
	if(mouse->x > mScreenPosition.x && mouse->x < mScreenPosition.x + mWidth * BLOCK_SIZE &&
		mouse->y > mScreenPosition.y && mouse->y < mScreenPosition.y + mHeight * BLOCK_SIZE){
		int mX = (int)(mouse->x - mScreenPosition.x) / BLOCK_SIZE;
		int mY = (int)(mouse->y - mScreenPosition.y) / BLOCK_SIZE;
		mousePos = sf::Vector2i((float)mX, (float)mY);

		if (mouseTracker->leftButton == Mouse::ButtonStateTracker::PRESSED){
			//if (mGrid[mY * mWidth + mX])
			//	delete mGrid[mY * mWidth + mX];

			mGrid[mY * mWidth + mX].reset(new Block(mousePos, DirectX::SimpleMath::Vector4(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 1.0f)));
		}
	}
#endif
	*/

	if (keyboardTracker->isKeyDown(sf::Keyboard::LControl) || keyboardTracker->isKeyDown(sf::Keyboard::RControl)){
		if (keyboardTracker->isKeyPressed(sf::Keyboard::M))
			switchAIMode();

		if (keyboardTracker->isKeyPressed(sf::Keyboard::S))
			saveGenomes();
		if (keyboardTracker->isKeyPressed(sf::Keyboard::L))
			loadGenomes();
	}
	//if (inputGamePad->back == inputGamePad->PRESSED)
	//	switchAIMode();


	//Let the AI control the Button presses
	/*
	if (AIMode){

		m_framesSinceLastTetromino++;
		if (m_framesSinceLastTetromino > 60 * 60)
			restart();

		AIList[CurrentAI]->update();
		auto state = AIList[CurrentAI]->m_controller->GetState();
		inputGamePad->Update(state);

	}
	*/
	/*else{
		m_AILearningFrameCounter++;

		//Check whether any button got pressed
		bool buttonPressed = false;
		buttonPressed |= (inputGamePad->dpadUp == inputGamePad->PRESSED);
		buttonPressed |= (inputGamePad->dpadDown == inputGamePad->PRESSED);
		buttonPressed |= (inputGamePad->dpadLeft == inputGamePad->PRESSED);
		buttonPressed |= (inputGamePad->dpadRight == inputGamePad->PRESSED);
		buttonPressed |= (inputGamePad->x == inputGamePad->PRESSED);
		buttonPressed |= (inputGamePad->y == inputGamePad->PRESSED);

		buttonPressed |= keyboardTracker->IsKeyPressed(Keyboard::Up);
		buttonPressed |= keyboardState->Down;
		buttonPressed |= keyboardTracker->IsKeyPressed(Keyboard::Left);
		buttonPressed |= keyboardTracker->IsKeyPressed(Keyboard::Right);
		buttonPressed |= keyboardTracker->IsKeyPressed(Keyboard::A);
		buttonPressed |= keyboardTracker->IsKeyPressed(Keyboard::D);

		if (buttonPressed || m_AILearningFrameCounter >= 20){
			for (unsigned i = 0; i < AICount; i++)
				AIList[i]->learn(inputGamePad, keyboardTracker, keyboardState);

			m_AILearningFrameCounter = 0;
		}
	}*/

	//Updatet den aktiven Tetromino.
	mTetrominoQueue[0]->update(keyboardTracker);

	//Wenn der aktive Tetromino gesetzt wurde, wird er gelöscht und die Tetrominos in der Schlange rücken nach. Ein neuer reiht sich hinten ein.
	if (mTetrominoQueue[0]->shouldDestroy()){

		mTetrominoQueue.erase(mTetrominoQueue.begin());
		//mTetrominoQueue.push_back(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(rand() % NUMBER_OF_MINOS + 1)));
		drawFromBag();

		mPoints += 50;
		m_framesSinceLastTetromino = 0;
	}



	//Update pressed buttons

	pressedButtons[0] = keyboardTracker->isKeyDown(sf::Keyboard::Up);
	pressedButtons[1] = keyboardTracker->isKeyDown(sf::Keyboard::Down);
	pressedButtons[2] = keyboardTracker->isKeyDown(sf::Keyboard::Left);
	pressedButtons[3] = keyboardTracker->isKeyDown(sf::Keyboard::Right);
	pressedButtons[4] = keyboardTracker->isKeyDown(sf::Keyboard::D);
	pressedButtons[5] = keyboardTracker->isKeyDown(sf::Keyboard::A);
}

void Field::Render(sf::RenderWindow* window){
	//Zeichnet den Hintergrund
	//TextureManager::getTexture(TEX_BACKGROUND_1)->draw(spritebatch, sf::Vector2i(0.0f, 0.0f));
	window->draw(m_backgroundSprite);

	//Texture* borderTexture = TextureManager::getTexture(TEX_BACKGROUND_FIELD_BORDER);
	sf::Vector2i borderOffset;
	borderOffset.x = (mWidth * BLOCK_SIZE - m_backgroundBorderSprite.getTexture()->getSize().x) / 2.0f;
	borderOffset.y = (mHeight * BLOCK_SIZE - m_backgroundBorderSprite.getTexture()->getSize().y) / 2.0f;

	m_backgroundBorderSprite.setPosition(mScreenPosition.x + borderOffset.x, mScreenPosition.y + borderOffset.y);

	//borderTexture->draw(spritebatch, mScreenPosition + borderOffset);
	window->draw(m_backgroundBorderSprite);

	//Render the overlay
	//TextureManager::getTexture(TEX_BACKGROUND_OVERLAY)->draw(spritebatch, sf::Vector2i(0.0f, 0.0f));
	m_backgroundOverlaySprite.setPosition(0.0f, 0.0f);
	window->draw(m_backgroundOverlaySprite);

	//Zeichnet transparente Blöcke als Spielfeldhintergrund.
	for (int y = 0; y < mHeight; y++){
		for (int x = 0; x < mWidth; x++){
			sf::Vector2f pos = mScreenPosition + sf::Vector2f((float)x, (float)y) * (float)BLOCK_SIZE;
			//TextureManager::getTexture(TEX_BACKGROUND_FIELD_TILE)->draw(spritebatch, pos, BLOCK_COLOR_WHITE * COLOR_ALPHA_25);
			m_backgroudFieldTileSprite.setPosition(pos.x, pos.y);
			m_backgroudFieldTileSprite.setColor(BLOCK_COLOR_WHITE * COLOR_ALPHA_25);
		}
	}

	//Zeichnet die auf dem Spielfeld platzierten Blöcke.
	for (int y = 0; y < mHeight; y++){
		for (int x = 0; x < mWidth; x++){
			if (mGrid[y * mWidth + x]){
				mGrid[y * mWidth + x]->render(window);
			}
		}
	}

	//Zeichnet den aktiven Tetromino
	mTetrominoQueue[0]->render(window);

	//Zeichnet die eingereiten tetrominos neben das Spielfeld.
	for (UINT i = 1; i < mTetrominoQueue.size(); i++){
		mTetrominoQueue[i]->render(window, mScreenPosition + sf::Vector2f(float((mWidth + 2) * BLOCK_SIZE), float((i - 1) * BLOCK_SIZE * 3.5f)), 0.75f);
	}



	
	//Render pressed buttons overlay

	sf::IntRect rect;
	rect.width = TEX_KEY_OVERLAY_SIZE.x;
	rect.height = TEX_KEY_OVERLAY_SIZE.y;
	
	//CCW
	sf::Vector2i off = pressedButtons[5] ? TEX_KEY_OVERLAY_CCW_PRESSED_RECT : TEX_KEY_OVERLAY_CCW_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	//TextureManager::getTexture(TEX_KEY_OVERLAY)->draw(spritebatch, keyOverlayOffset + sf::Vector2i(0, 0), rect);
	m_keyCounterClockwiseSprite.setTextureRect(rect);
	window->draw(m_keyCounterClockwiseSprite);

	//CW
	off = pressedButtons[4] ? TEX_KEY_OVERLAY_CW_PRESSED_RECT : TEX_KEY_OVERLAY_CW_RELEASED_RECT;

	rect.left =   static_cast<LONG>(off.x);
	rect.top =    static_cast<LONG>(off.y);

	//TextureManager::getTexture(TEX_KEY_OVERLAY)->draw(spritebatch, keyOverlayOffset + sf::Vector2i(128, 0), rect
	window->draw(m_keyClockwiseSprite);


	//UP
	off = pressedButtons[0] ? TEX_KEY_OVERLAY_UP_PRESSED_RECT : TEX_KEY_OVERLAY_UP_RELEASED_RECT;
	
	rect.left =   static_cast<LONG>(off.x);
	rect.top =    static_cast<LONG>(off.y);

	//TextureManager::getTexture(TEX_KEY_OVERLAY)->draw(spritebatch, keyOverlayOffset + sf::Vector2i(64, 0), rect);
	window->draw(m_keyUpSprite);

	//DOWN
	off = pressedButtons[1] ? TEX_KEY_OVERLAY_DOWN_PRESSED_RECT : TEX_KEY_OVERLAY_DOWN_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	//TextureManager::getTexture(TEX_KEY_OVERLAY)->draw(spritebatch, keyOverlayOffset + sf::Vector2i(64, 64), rect);
	window->draw(m_keyDownSprite);

	//LEFT
	off = pressedButtons[2] ? TEX_KEY_OVERLAY_LEFT_PRESSED_RECT : TEX_KEY_OVERLAY_LEFT_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	//TextureManager::getTexture(TEX_KEY_OVERLAY)->draw(spritebatch, keyOverlayOffset + sf::Vector2i(0, 64), rect);
	window->draw(m_keyLeftSprite);

	//RIGHT
	off = pressedButtons[3] ? TEX_KEY_OVERLAY_RIGHT_PRESSED_RECT : TEX_KEY_OVERLAY_RIGHT_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	//TextureManager::getTexture(TEX_KEY_OVERLAY)->draw(spritebatch, keyOverlayOffset + sf::Vector2i(128, 64), rect);
	window->draw(m_keyRightSprite);

	

	//Render text
	sf::Vector2f pos = mScreenPosition + sf::Vector2f(float((mWidth + 6) * BLOCK_SIZE), 0);
	sf::Text text;

	std::wstringstream str;
	str.clear();
	str << L"Points:             " << mPoints;

	//spriteFont->DrawString(spritebatch, str.str().c_str(), pointStringPos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Highscore:         " << m_Highscore;

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"HighscoreAI:     " << m_HighscoreAI;

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	pos.y += 20;

	str = std::wstringstream();
	str.clear();
	str << L"Lines:              " << mTotalLinesCleared;

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Highest lines:     " << m_totalLinesHighscore;

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Highest linesAI: " << m_totalLinesHighscoreAI;

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);



	pos.y += 40;
	str = std::wstringstream();
	str.clear();
	str << L"Genome: " << (CurrentAI + 1) << "/" << AICount;

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Generation: " << (CurrentGen + 1);

	pos.y += 20;
	//spriteFont->DrawString(spritebatch, str.str().c_str(), pos, Colors::Black);
	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);
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
				mGrid[y * mWidth + x]->setPosition(sf::Vector2i((float)x, (float)y));
		}
	}

	mPoints += 1000 * linesCleared * linesCleared;

	mTotalLinesCleared += linesCleared;
	mGravity = mTotalLinesCleared / 10.0f + 1.0f;
}

void Field::clearLine(int y, int& linesCleared){
	for (int x = 0; x < mWidth; x++){
		//delete mGrid[y * mWidth + x];
		//mGrid[y * mWidth + x] = nullptr;
		mGrid[y * mWidth + x].reset();
	}
	linesCleared++;

	for (int j = y; j > 0; j--){
		for (int i = 0; i < mWidth; i++){
			mGrid[j * mWidth + i] = mGrid[(j - 1) * mWidth + i];
		}
	}
	for (int i = 0; i < mWidth; i++)
		mGrid[i].reset();
}

void Field::addBlock(std::shared_ptr<Block> block){
	int x = (int)block->getPosition().x;
	int y = (int)block->getPosition().y;

	if (x < 0 || y < 0){
		//restart();
		return;
	}

	mGrid[y * mWidth + x].reset();

	mGrid[y * mWidth + x] = block;
}


