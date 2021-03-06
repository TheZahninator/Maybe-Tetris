#include <pch.h>
#include "Field.h"


Field::Field() :
	mWidth(10), mHeight(20), m_copiesInBag(2), m_framesSinceLastTetromino(0), m_AILearningFrameCounter(0), m_lastTetrominoType(0)
{
	AICount = 25;
	CurrentAI = 0;
	SurvivingAIs = 2;
	CurrentGen = 0;
}

void Field::fillBag() {

	for (unsigned i = 0; i < m_copiesInBag; i++) {
		for (int j = 0; j < NUMBER_OF_MINOS; j++) {
			m_bag.add(std::shared_ptr<TetrominoGroup>(new TetrominoGroup(*this, j)));
		}
	}
}

void Field::drawFromBag() {
	std::shared_ptr<TetrominoGroup> group;

	do {
		if (!mTetrominoQueue.empty())
			m_lastTetrominoType = mTetrominoQueue.back()->getType();
		group = m_bag.draw();

		if (m_bag.isEmpty())
			fillBag();
	} while (group->getType() == m_lastTetrominoType);

	mTetrominoQueue.push_back(group);

}

void Field::init(sf::Vector2f& screenPosition) {
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
	m_bag.clear();

	fillBag();

	for (int i = 0; i < QUEUE_SIZE; i++) {
		drawFromBag();
	}

	m_gravity = 1.0f;

	m_score = 0;
	m_Highscore = 0;
	m_HighscoreAI = 0;

	m_totalLinesCleared = 0;
	m_totalLinesHighscore = 0;
	m_totalLinesHighscoreAI = 0;

	for (unsigned i = 0; i < AICount; i++) {
		AIList.push_back(std::unique_ptr<AI>(new AI(*this)));
	}

	AIMode = false;
	m_trainingMode = false;

	m_framesSinceLastTetromino = 0;

	for (unsigned i = 0; i < 6; i++)
		pressedButtons[i] = false;

	m_recordMode = false;

	restart();
}

void Field::restart() {
	if (m_recordMode) {
		char c;

		do {
			std::cout << "Save replay? <Y/n>" << std::endl;

			c = std::getchar();
		} while (c != 'n' && c != 'Y' && c != '\n');

		if (c == 'Y' || c == '\n') {
			saveReplay();
			std::cout << "Replay saved" << std::endl;
		}

		m_replayData.clear();
	}

	if (mGrid)
		delete[] mGrid;

	mTetrominoQueue.clear();

	mGrid = new std::shared_ptr<Block>[mWidth * mHeight];

	m_bag.clear();

	fillBag();

	for (int i = 0; i < QUEUE_SIZE; i++) {
		drawFromBag();
	}

	if (AIMode) {
		AIList[CurrentAI]->updateFitness(m_score);
		CurrentAI++;

		if (CurrentAI >= AICount) {
			NewGen();

			CurrentAI = 0;
		}


		if (m_score > m_HighscoreAI)
			m_HighscoreAI = m_score;

		if (m_totalLinesCleared > m_totalLinesHighscoreAI)
			m_totalLinesHighscoreAI = m_totalLinesCleared;
	}
	else {
		if (m_score > m_Highscore)
			m_Highscore = m_score;

		if (m_totalLinesCleared > m_totalLinesHighscore)
			m_totalLinesHighscore = m_totalLinesCleared;
	}

	m_gravity = 1.0f;
	m_score = 0;
	m_totalLinesCleared = 0;
	m_framesSinceLastTetromino = 0;

	m_AILearningFrameCounter = 0;

	m_recordModeFrameCount = 0;
}

void Field::switchAIMode()
{
	restart();

	AIMode = !AIMode;
	CurrentAI = 0;

	std::cout << "AI mode: " << (AIMode ? "on" : "off") << std::endl;
}

void Field::switchTrainingMode()
{
	restart();

	m_trainingMode = !m_trainingMode;

	std::cout << "Training mode: " << (m_trainingMode ? "on" : "off") << std::endl;
}

void Field::switchRecordMode()
{
	restart();

	m_recordMode = !m_recordMode;

	std::cout << "Recording: " << (m_recordMode ? "on" : "off") << std::endl;
}

void Field::NewGen() {
	if (AIList.size() < SurvivingAIs)
		SurvivingAIs = AIList.size();

	std::vector<std::unique_ptr<AI>> survived;
	survived.clear();

	while (survived.size() < SurvivingAIs) {
		AI* highestFitness = AIList.front().get();
		unsigned x = 0;

		for (unsigned i = 1; i < AIList.size(); i++) {
			if (AIList[i]->m_fitness > highestFitness->m_fitness) {
				highestFitness = AIList[i].get();
				x = i;
			}
		}

		survived.push_back(std::move(AIList[x]));
		AIList.erase(AIList.begin() + x);
	}

	//Clear AI list
	AIList.clear();

	//Write surviving AIs back into the list.
	for (unsigned i = 0; i < survived.size(); i++) {
		AIList.push_back(std::move(survived[i]));
		AIList.back()->updateFitness(0);
	}

	//Calculate chances for every surviver to procreate.
	float chanceScale = 1.5;

	float totalChance = 0;

	std::vector<float> chances;
	chances.push_back(1);

	totalChance += 1;

	for (unsigned i = 1; i < SurvivingAIs; i++) {
		float chance = chances[i - 1] / chanceScale;
		totalChance += chance;
		chances.push_back(chance);
	}

	while (AIList.size() < AICount) {
		int num = rand();
		float result = static_cast<float>(ZahnNN::map(static_cast<double>(num), 0.0, static_cast<double>(RAND_MAX), 0.0, static_cast<double>(totalChance)));

		float cumlutativeChance = totalChance;
		unsigned i = 0;
		for (i = 0; i < chances.size(); i++) {
			cumlutativeChance -= chances[i];
			if (result >= cumlutativeChance) {

				break;
			}
		}

		cumlutativeChance = totalChance;
		unsigned j = 0;
		for (j = 0; j < chances.size(); j++) {
			cumlutativeChance -= chances[j];
			if (result >= cumlutativeChance) {

				break;
			}
		}
		AIList.push_back(std::unique_ptr<AI>(AIList[i]->recreate(AIList[j].get())));
	}
	CurrentAI = 0;
	CurrentGen++;
}

void Field::saveReplay()
{
	std::cout << "Saving replay..." << std::endl;

	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("replay");

	pugi::xml_attribute atr = root.append_attribute("data_count");
	atr.set_value(m_replayData.size());

	atr = root.append_attribute("score");
	atr.set_value(m_score);

	atr = root.append_attribute("lines");
	atr.set_value(m_totalLinesCleared);

	for (ReplayData replayData : m_replayData) {
		pugi::xml_node data = root.append_child("data");

		atr = data.append_attribute("frame");
		atr.set_value(replayData.frameNumber);

		atr = data.append_attribute("spawned_new");
		atr.set_value(replayData.newTetromino);

		pugi::xml_node buttons = data.append_child("buttons");
		unsigned i = 0;
		for (bool button : replayData.pressedButtons) {
			pugi::xml_node button_node = buttons.append_child("button");

			atr = button_node.append_attribute("index");
			atr.set_value(i++);

			atr = button_node.append_attribute("pressed");
			atr.set_value(button);
		}

		pugi::xml_node queue = data.append_child("queue");
		i = 0;
		for (int type : replayData.tetrominoQueue) {
			pugi::xml_node tetromino = queue.append_child("tetromino");

			atr = tetromino.append_attribute("index");
			atr.set_value(i++);

			atr = tetromino.append_attribute("type");
			atr.set_value(type);
		}

		pugi::xml_node input_node = data.append_child("input");

		for (unsigned i = 0; i < replayData.inputData.size(); i++) {
			pugi::xml_node value = input_node.append_child("value");

			atr = value.append_attribute("index");
			atr.set_value(i);

			atr = value.append_attribute("val");
			atr.set_value(replayData.inputData[i]);
		}
	}

	std::stringstream str;
	str << "res/replays/replay_" << std::time(nullptr) << ".xml";

	doc.save_file(str.str().c_str());
}

void Field::saveBest()
{
	pugi::xml_document doc;
	pugi::xml_node root = doc.append_child("root");

	pugi::xml_attribute atr = root.append_attribute("memory");
	atr.set_value(AI::MemorySize);

	ZahnNN::NeuralNet* best_net = AIList.front()->getNet().get();

	pugi::xml_node ai = root.append_child("AI");

	for (unsigned j = 0; j < best_net->getLayerCount(); j++) {
		ZahnNN::Layer& layer = best_net->getLayer(j);
		pugi::xml_node layerNode = ai.append_child("Layer");

		bool isOutputLayer = j == best_net->getLayerCount() - 1;

		pugi::xml_attribute size = layerNode.append_attribute("size");
		size.set_value(layer.size() - (unsigned)!isOutputLayer);

		for (unsigned k = 0; k < layer.size() - (unsigned)isOutputLayer; k++) {
			ZahnNN::Neuron& neuron = layer[k];
			pugi::xml_node neuronNode = layerNode.append_child("neuron");

			for (unsigned l = 0; l < neuron.getOutputWeights().size(); l++) {
				double weight = neuron.getOutputWeights()[l].weight;
				pugi::xml_attribute weightAtr = neuronNode.append_attribute("weight");
				weightAtr.set_value(weight);
			}
		}

		if (!isOutputLayer) {
			ZahnNN::Neuron& bias = layer.back();
			pugi::xml_node biasNode = layerNode.append_child("bias");

			for (unsigned l = 0; l < bias.getOutputWeights().size(); l++) {
				double weight = bias.getOutputWeights()[l].weight;
				pugi::xml_attribute weightAtr = biasNode.append_attribute("weight");
				weightAtr.set_value(weight);
			}
		}
	}

	doc.save_file("res/save.xml");

	std::cout << "Saved best network" << std::endl;
}

void Field::loadBest() {
	pugi::xml_document doc;
	doc.load_file("res/save.xml");

	pugi::xml_node root = doc.child("root");
	/*pugi::xml_attribute atr = root.attribute("popSize");
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
		AI::MemorySize = atr.as_uint();*/

	AIList.clear();

	AIList.push_back(std::unique_ptr<AI>(new AI(*this)));

	ZahnNN::NeuralNet* net = AIList.back()->getNet().get();

	pugi::xml_node ai = root.child("AI");

	int i = 0;
	for (auto layer : ai.children("Layer"))
	{
		int j = 0;
		for (auto neuron : layer.children("neuron"))
		{
			int k = 0;
			for (auto weight : neuron.attributes())
			{
				net->getLayer(i)[j].getOutputWeights()[k].weight = weight.as_double();

				k++;
			}
			j++;
		}

		for (auto bias : layer.children("bias"))
		{

			int k = 0;
			for (auto weight : bias.attributes())
			{
				net->getLayer(i).back().getOutputWeights()[k].weight = weight.as_double();

				k++;
			}
			j++;
		}
		i++;
	}

	NewGen();
	restart();
	CurrentAI = 0;

	std::cout << "Loaded network" << std::endl;
}

void Field::nextTetromino() {
	drawFromBag();
	m_spawnedNewTetromino = true;
	if (!mTetrominoQueue[0]->move(sf::Vector2i(0, 0))) {
		restart();
	}
}

void Field::Update(KeyboardStateTracker* playerKeyboardTracker)
{
	m_spawnedNewTetromino = false;

	if (playerKeyboardTracker->isKeyDown(sf::Keyboard::LControl) || playerKeyboardTracker->isKeyDown(sf::Keyboard::RControl)) {
		if (playerKeyboardTracker->isKeyPressed(sf::Keyboard::M))
			switchAIMode();
		if (playerKeyboardTracker->isKeyPressed(sf::Keyboard::T))
			switchTrainingMode();
		if (playerKeyboardTracker->isKeyPressed(sf::Keyboard::R))
			switchRecordMode();

		if (playerKeyboardTracker->isKeyPressed(sf::Keyboard::S))
			saveBest();
		if (playerKeyboardTracker->isKeyPressed(sf::Keyboard::L))
			loadBest();
	}

	KeyboardStateTracker* currentKeyboard = playerKeyboardTracker;

	//Let the AI control the Button presses

	if (AIMode) {

		m_framesSinceLastTetromino++;
		if (m_framesSinceLastTetromino > 60 * 60)
			restart();

		AIList[CurrentAI]->update();
		currentKeyboard = &AIList[CurrentAI]->getKeyboardStateTracker();
	}

	else {
		if (m_trainingMode) {
			m_AILearningFrameCounter++;

			//Check whether any button got pressed
			bool buttonPressed = false;

			buttonPressed |= currentKeyboard->isKeyPressed(sf::Keyboard::Up);
			buttonPressed |= currentKeyboard->isKeyDown(sf::Keyboard::Down);
			buttonPressed |= currentKeyboard->isKeyPressed(sf::Keyboard::Left);
			buttonPressed |= currentKeyboard->isKeyPressed(sf::Keyboard::Right);
			buttonPressed |= currentKeyboard->isKeyPressed(sf::Keyboard::A);
			buttonPressed |= currentKeyboard->isKeyPressed(sf::Keyboard::D);

			if (buttonPressed) { // || m_AILearningFrameCounter >= 20){
				AIList.front()->learn(currentKeyboard);

				m_AILearningFrameCounter = 0;
			}
		}
	}

	//Update the active tetromino
	mTetrominoQueue[0]->update(currentKeyboard);

	if (mTetrominoQueue[0]->shouldDestroy()) {

		mTetrominoQueue.erase(mTetrominoQueue.begin());
		nextTetromino();

		m_score += 50;
		m_framesSinceLastTetromino = 0;
	}

	bool buttonsPressedLastFrame[6];
	for (unsigned i = 0; i < 6; i++) {
		buttonsPressedLastFrame[i] = pressedButtons[i];
	}

	//Update pressed buttons
	pressedButtons[0] = currentKeyboard->isKeyDown(sf::Keyboard::Up);//|| currentKeyboard->isKeyPressed(sf::Keyboard::Up);
	pressedButtons[1] = currentKeyboard->isKeyDown(sf::Keyboard::Down);//|| currentKeyboard->isKeyPressed(sf::Keyboard::Down);
	pressedButtons[2] = currentKeyboard->isKeyDown(sf::Keyboard::Left);//|| currentKeyboard->isKeyPressed(sf::Keyboard::Left);
	pressedButtons[3] = currentKeyboard->isKeyDown(sf::Keyboard::Right);//|| currentKeyboard->isKeyPressed(sf::Keyboard::Right);
	pressedButtons[4] = currentKeyboard->isKeyDown(sf::Keyboard::D);//|| currentKeyboard->isKeyPressed(sf::Keyboard::D);
	pressedButtons[5] = currentKeyboard->isKeyDown(sf::Keyboard::A);//|| currentKeyboard->isKeyPressed(sf::Keyboard::A);

	if (m_recordMode)
	{
		ReplayData currentFrameData;

		currentFrameData.frameNumber = m_recordModeFrameCount++;
		currentFrameData.newTetromino = m_spawnedNewTetromino;

		for (unsigned i = 0; i < 6; i++)
		{
			currentFrameData.pressedButtons[i] = pressedButtons[i];
		}

		for (unsigned i = 0; i < mTetrominoQueue.size(); i++)
		{
			currentFrameData.tetrominoQueue[i] = mTetrominoQueue[i]->getType();
		}

		auto frontAI = AIList.front().get();

		for (unsigned i = 0; i < frontAI->getInputData().size() - frontAI->NumButtons; i++)
		{
			currentFrameData.inputData.push_back(frontAI->getInputData()[i]);
		}

		for (unsigned i = 0; i < 6; i++)
		{
			currentFrameData.inputData.push_back(buttonsPressedLastFrame[i] ? 1.0 : 0.0);
		}

		m_replayData.push_back(currentFrameData);
	}
}

void Field::Render(sf::RenderWindow* window)
{
	//Render the background
	window->draw(m_backgroundSprite);

	//Render the field border
	sf::Vector2f borderOffset;
	borderOffset.x = (mWidth * BLOCK_SIZE - m_backgroundBorderSprite.getTexture()->getSize().x) / 2.0f;
	borderOffset.y = (mHeight * BLOCK_SIZE - m_backgroundBorderSprite.getTexture()->getSize().y) / 2.0f;

	m_backgroundBorderSprite.setPosition(mScreenPosition + borderOffset);

	window->draw(m_backgroundBorderSprite);

	//Render the overlay
	m_backgroundOverlaySprite.setPosition(0.0f, 0.0f);
	window->draw(m_backgroundOverlaySprite);

	for (unsigned y = 0; y < mHeight; y++) {
		for (unsigned x = 0; x < mWidth; x++) {
			sf::Vector2f pos = mScreenPosition + sf::Vector2f((float)x, (float)y) * (float)BLOCK_SIZE;
			m_backgroudFieldTileSprite.setPosition(pos.x, pos.y);
			m_backgroudFieldTileSprite.setColor(BLOCK_COLOR_WHITE * COLOR_ALPHA_25);
			window->draw(m_backgroudFieldTileSprite);
		}
	}

	for (unsigned y = 0; y < mHeight; y++) {
		for (unsigned x = 0; x < mWidth; x++) {
			if (mGrid[y * mWidth + x]) {
				mGrid[y * mWidth + x]->render(window);
			}
		}
	}

	mTetrominoQueue[0]->render(window);

	for (UINT i = 1; i < mTetrominoQueue.size(); i++) {
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

	m_keyCounterClockwiseSprite.setTextureRect(rect);
	window->draw(m_keyCounterClockwiseSprite);

	//CW
	off = pressedButtons[4] ? TEX_KEY_OVERLAY_CW_PRESSED_RECT : TEX_KEY_OVERLAY_CW_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	m_keyClockwiseSprite.setTextureRect(rect);
	window->draw(m_keyClockwiseSprite);


	//UP
	off = pressedButtons[0] ? TEX_KEY_OVERLAY_UP_PRESSED_RECT : TEX_KEY_OVERLAY_UP_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	m_keyUpSprite.setTextureRect(rect);
	window->draw(m_keyUpSprite);

	//DOWN
	off = pressedButtons[1] ? TEX_KEY_OVERLAY_DOWN_PRESSED_RECT : TEX_KEY_OVERLAY_DOWN_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	m_keyDownSprite.setTextureRect(rect);
	window->draw(m_keyDownSprite);

	//LEFT
	off = pressedButtons[2] ? TEX_KEY_OVERLAY_LEFT_PRESSED_RECT : TEX_KEY_OVERLAY_LEFT_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	m_keyLeftSprite.setTextureRect(rect);
	window->draw(m_keyLeftSprite);

	//RIGHT
	off = pressedButtons[3] ? TEX_KEY_OVERLAY_RIGHT_PRESSED_RECT : TEX_KEY_OVERLAY_RIGHT_RELEASED_RECT;

	rect.left = static_cast<LONG>(off.x);
	rect.top = static_cast<LONG>(off.y);

	m_keyRightSprite.setTextureRect(rect);
	window->draw(m_keyRightSprite);

	//Render text
	float xLeft = mScreenPosition.x + (mWidth + 7) * BLOCK_SIZE;
	float xRight = static_cast<float>(m_screenSize.x - 150);
	sf::Vector2f pos = mScreenPosition;

	sf::Font font;
	font.loadFromFile("res/arial.ttf");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::Black);

	std::wstringstream str;
	str.clear();
	str << L"Points:";

	pos.x = xLeft;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << m_score;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Highscore:";

	pos.y += 20;
	pos.x = xLeft;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << m_Highscore;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"HighscoreAI:";

	pos.y += 20;
	pos.x = xLeft;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << m_HighscoreAI;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Lines:";

	pos.y += 40;
	pos.x = xLeft;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << m_totalLinesCleared;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Highest lines:";

	pos.y += 20;
	pos.x = xLeft;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << m_totalLinesHighscore;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << L"Highest linesAI:";

	pos.y += 20;
	pos.x = xLeft;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << m_totalLinesHighscoreAI;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);


	pos.x = xLeft;
	pos.y += 40;
	str = std::wstringstream();
	str.clear();
	str << L"Genome:";

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << (CurrentAI + 1) << "/" << AICount;

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);

	pos.y += 20;
	pos.x = xLeft;

	str = std::wstringstream();
	str.clear();
	str << L"Generation:";

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(0, 0);
	window->draw(text);

	str = std::wstringstream();
	str.clear();
	str << (CurrentGen + 1);

	pos.x = xRight;

	text.setString(sf::String(str.str()));
	text.setPosition(pos);
	text.setOrigin(text.getLocalBounds().width, 0);
	window->draw(text);
}

void Field::checkForLineClear() {
	int linesCleared = 0;

	for (unsigned y = 0; y < mHeight; y++) {

		int blocksInLine = 0;

		for (unsigned x = 0; x < mWidth; x++) {
			if (mGrid[y * mWidth + x]) {
				blocksInLine++;
			}

			if (blocksInLine >= 10) {
				clearLine(y, linesCleared);
			}
		}
	}

	for (unsigned y = 0; y < mHeight; y++) {
		for (unsigned x = 0; x < mWidth; x++) {
			if (mGrid[y * mWidth + x])
				mGrid[y * mWidth + x]->setPosition(sf::Vector2i(x, y));
		}
	}

	m_score += 1000 * linesCleared * linesCleared;

	m_totalLinesCleared += linesCleared;
	m_gravity = m_totalLinesCleared / 10.0f + 1.0f;
}

void Field::clearLine(int y, int& linesCleared) {
	for (unsigned x = 0; x < mWidth; x++) {
		//delete mGrid[y * mWidth + x];
		//mGrid[y * mWidth + x] = nullptr;
		mGrid[y * mWidth + x].reset();
	}
	linesCleared++;

	for (unsigned j = y; j > 0; j--) {
		for (unsigned i = 0; i < mWidth; i++) {
			mGrid[j * mWidth + i] = mGrid[(j - 1) * mWidth + i];
		}
	}
	for (unsigned i = 0; i < mWidth; i++)
		mGrid[i].reset();
}

void Field::addBlock(std::shared_ptr<Block> block) {
	int x = (int)block->getPosition().x;
	int y = (int)block->getPosition().y;

	if (x < 0 || y < 0) {
		//restart();
		return;
	}

	mGrid[y * mWidth + x].reset();

	mGrid[y * mWidth + x] = block;
}


