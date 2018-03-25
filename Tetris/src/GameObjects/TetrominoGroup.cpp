#include <pch.h>
#include "TetrominoGroup.h"


TetrominoGroup::TetrominoGroup(Field& field, int type) : mSplitted(false), mToBeDestroyed(false), m_field(field){
	m_type = type;

	int w = 0;
	int h = 0;

	bool* matrix = nullptr;
	sf::Color color = sf::Color::White;
	

	switch (type){
	case 0: //I
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_MINO_I;
		color = BLOCK_COLOR_I;
		break;

	case 1: //T
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_T;
		color = BLOCK_COLOR_T;
		break;

	case 2: //L
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_L;
		color = BLOCK_COLOR_L;
		break;

	case 3: //J
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_J;
		color = BLOCK_COLOR_J;
		break;

	case 4: //Z
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_Z;
		color = BLOCK_COLOR_Z;
		break;

	case 5: //S
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_S;
		color = BLOCK_COLOR_S;
		break;

	case 6: //O
		w = 2;
		h = 2;
		matrix = new bool[w * h]MAT_MINO_O;
		color = BLOCK_COLOR_O;
		break;

		//SPECIAL MINOS
	case 7: //1
		w = 5;
		h = 5;
		matrix = new bool[w * h]MAT_SPECIAL_1;
		break;

	case 8:
		w = 5;
		h = 5;
		matrix = new bool[w * h]MAT_SPECIAL_2;
		break;

	case 9:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_3;
		break;

	case 10:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_4;
		break;

	case 11:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_5;
		break;

	case 12:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_6;
		break;

	case 13:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_7;
		break;

	case 14:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_8;
		break;

	case 15:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_9;
		break;

	case 16:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_10;
		break;

	case 17:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_11;
		break;

	case 18:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_12;
		break;

	case 19:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_13;
		break;

	case 20:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_14;
		break;

	case 21:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_15;
		break;

	case 22:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_16;
		break;

	case 23:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_17;
		break;

	case 24:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_18;
		break;

	case 25:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_19;
		break;

	case 26:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_20;
		break;

	case 27:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_21;
		break;

	case 28:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_22;
		break;

	case 29:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_23;
		break;

	case 30:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_24;
		break;

	case 31:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_25;
		break;

	case 32:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_26;
		break;

	case 33:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_27;
		break;

	case 34:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_28;
		break;

	case 35:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_29;
		break;

	case 36:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_30;
		break;

	case 37:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_31;
		break;

	case 38:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_32;
		break;

	}

	mPositionGrid.x = (float)(m_field.getWidth() / 2 - (int)(w / 2));
	mPositionGrid.y = (float)(2 - h);

	mTetrominos.push_back(std::shared_ptr<Tetromino>(new Tetromino(m_field, mPositionGrid, matrix, w, h, color)));
	if (mTetrominos[0]->collidedAtSpawn()){
		m_field.restart();
	}

	m_rotation = 0;
}


TetrominoGroup::~TetrominoGroup(){
}

void TetrominoGroup::update(KeyboardStateTracker* keyboardTracker){
	
	bool tillAllocked = false;
	if (keyboardTracker->isKeyPressed(sf::Keyboard::Up))
		tillAllocked = true;

	bool allLocked = true;

	do{
		allLocked = true;
		for (UINT i = 0; i < mTetrominos.size(); i++){
			mTetrominos[i]->update(keyboardTracker);
			if (!mSplitted){
				allLocked = false;
				if (mTetrominos[i]->isLocked()){
					split(mTetrominos[i].get());
					break;
				}
			}
			else{
				allLocked &= mTetrominos[i]->isLocked();
				if (mTetrominos[i]->isLocked()){
					mTetrominos[i]->placeOnField();
					mTetrominos.erase(mTetrominos.begin() + i);
					i--;
				}
			}
		}
	} while (tillAllocked && !allLocked);

	if (!mSplitted){
		//TODO: Input handling
		if (keyboardTracker->isKeyPressed(sf::Keyboard::A))
			rotate(ERotation::COUNTER_CLOCKWISE);
		else if (keyboardTracker->isKeyPressed(sf::Keyboard::D))
			rotate(ERotation::CLOCKWISE);

		if (keyboardTracker->isKeyPressed(sf::Keyboard::Left))
			move(sf::Vector2i(-1, 0));
		if (keyboardTracker->isKeyPressed(sf::Keyboard::Right))
			move(sf::Vector2i(1, 0));
	}
	else if(allLocked){
		//TODO: Blöcke auf das Spielfeld überschreiben und neuen Tetromino spawnen.
		for (UINT i = 0; i < mTetrominos.size(); i++)
			mTetrominos[i]->placeOnField();

		mToBeDestroyed = true;

		m_field.checkForLineClear();
	}
}

void TetrominoGroup::render(sf::RenderWindow* window){
	for (UINT i = 0; i < mTetrominos.size(); i++){
		mTetrominos[i]->render(window);
	}
}

void TetrominoGroup::render(sf::RenderWindow* window, const sf::Vector2f& screenPosition, float scale){
	for (UINT i = 0; i < mTetrominos.size(); i++){
		mTetrominos[i]->render(window, screenPosition, scale);
	}
}

void TetrominoGroup::rotate(ERotation rotation){
	if (rotation == ERotation::CLOCKWISE)
		m_rotation++;
	else if (rotation == ERotation::COUNTER_CLOCKWISE)
		m_rotation--;

	if (m_rotation < 0)
		m_rotation = 3;
	else if (m_rotation > 3)
		m_rotation = 0;

	for (UINT i = 0; i < mTetrominos.size(); i++){
		mTetrominos[i]->rotate(rotation);
	}
}

bool TetrominoGroup::setPosition(sf::Vector2i& position){
	for (UINT i = 0; i < mTetrominos.size(); i++)
		mTetrominos[i]->setPosition(position);

	return true;
}

bool TetrominoGroup::move(sf::Vector2i& direction){
	bool successed = true;
	
	for (UINT i = 0; i < mTetrominos.size(); i++){
		successed &= mTetrominos[i]->move(direction);
	}
	return successed;
}

void TetrominoGroup::split(Tetromino* tetromino){
	std::vector<std::shared_ptr<Tetromino>> newMinos;	//Liste der neuen Tetrominos.
	
	int* matrix = tetromino->getFieldMatrix();	//Zwischenspeichern der Matrix.
	int width = (int)tetromino->getMatrixSize().x;	//Zwischenspeichern der Matrixbreite.
	int height = (int)tetromino->getMatrixSize().y;	//Zwischenspeichern der Matrixhöhe.
	bool* checked = new bool[width * height];	//Matrix um zu markieren, welche Felder bereits geprüft wurden.
	memset(checked, 0, width * height);			//Initialisieren der Prüf-Matrix

	//Iteriert durch alle Felder der Matrix.
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			if (!checked[y * width + x]){	//Wenn das aktuelle Feld noch nicht überprüft wurde...
				checked[y * width + x] = true;	//Das aktuelle Feld wird als überprüft markiert.
				if (matrix[y * width + x] > 0){	//Wenn das Feld einen Block besitzt...
					//Der Teil-Tettromino wird der Liste hinzugefügt.
					newMinos.push_back(std::shared_ptr<Tetromino>(tetromino->getPart(matrix, checked, width, height, x, y)));
				}
			}

		}
	}


	mTetrominos = newMinos;
	mSplitted = true;

	delete[] checked;
}


int* TetrominoGroup::getMatrix(){ 
	return mTetrominos[0]->getOriginalMatrix(); 
}

sf::Vector2i TetrominoGroup::getMatrixSize() { 
	return mTetrominos[0]->getMatrixSize(); 
}

UINT TetrominoGroup::getEstimatedTouchingBlocks(){
	UINT result = 0;
	for (unsigned i = 0; i < mTetrominos.size(); i++){
		result += mTetrominos[i]->getEstimatedTouchingBlocks();
	}
	return result;
}

UINT TetrominoGroup::getTouchingBlocks(){
	UINT result = 0;
	for (unsigned i = 0; i < mTetrominos.size(); i++){
		result += mTetrominos[i]->getTouchingBlocks();
	}
	return result;
}