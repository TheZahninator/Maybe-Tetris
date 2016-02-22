#include "pch.h"
#include "TetrominoGroup.h"


TetrominoGroup::TetrominoGroup(int type) : mSplitted(false), mToBeDestroyed(false){
	int w;
	int h;

	bool* matrix;
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::White.v;
	

	switch (type){
	case 1: //I
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_MINO_I;
		color = BLOCK_COLOR_I;
		break;

	case 2: //T
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_T;
		color = BLOCK_COLOR_T;
		break;

	case 3: //L
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_L;
		color = BLOCK_COLOR_L;
		break;

	case 4: //J
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_J;
		color = BLOCK_COLOR_J;
		break;

	case 5: //Z
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_Z;
		color = BLOCK_COLOR_Z;
		break;

	case 6: //S
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_MINO_S;
		color = BLOCK_COLOR_S;
		break;

	case 7: //O
		w = 2;
		h = 2;
		matrix = new bool[w * h]MAT_MINO_O;
		color = BLOCK_COLOR_O;
		break;

		//SPECIAL MINOS
	case 8: //1
		w = 5;
		h = 5;
		matrix = new bool[w * h]MAT_SPECIAL_1;
		break;

	case 9:
		w = 5;
		h = 5;
		matrix = new bool[w * h]MAT_SPECIAL_2;
		break;

	case 10:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_3;
		break;

	case 11:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_4;
		break;

	case 12:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_5;
		break;

	case 13:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_6;
		break;

	case 14:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_7;
		break;

	case 15:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_8;
		break;

	case 16:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_9;
		break;

	case 17:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_10;
		break;

	case 18:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_11;
		break;

	case 19:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_12;
		break;

	case 20:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_13;
		break;

	case 21:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_14;
		break;

	case 22:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_15;
		break;

	case 23:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_16;
		break;

	case 24:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_17;
		break;

	case 25:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_18;
		break;

	case 26:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_19;
		break;

	case 27:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_20;
		break;

	case 28:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_21;
		break;

	case 29:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_22;
		break;

	case 30:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_23;
		break;

	case 31:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_24;
		break;

	case 32:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_25;
		break;

	case 33:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_26;
		break;

	case 34:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_27;
		break;

	case 35:
		w = 4;
		h = 4;
		matrix = new bool[w * h]MAT_SPECIAL_28;
		break;

	case 36:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_29;
		break;

	case 37:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_30;
		break;

	case 38:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_31;
		break;

	case 39:
		w = 3;
		h = 3;
		matrix = new bool[w * h]MAT_SPECIAL_32;
		break;

	}

	mPositionGrid.x = (float)(Field::getWidth() / 2 - (int)(w / 2));
	mPositionGrid.y = (float)(2 - h);

	mTetrominos.push_back(new Tetromino(mPositionGrid, matrix, w, h, color));
	if (mTetrominos[0]->collidedAtSpawn()){
		PostQuitMessage(0);
	}
}


TetrominoGroup::~TetrominoGroup(){
	for (UINT i = 0; i < mTetrominos.size(); i++)
		delete mTetrominos[i];
}

void TetrominoGroup::update(DirectX::GamePad::ButtonStateTracker* gamePadTracker, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState){
	
	bool tillAllocked = false;
	if (gamePadTracker->dpadUp == Mouse::ButtonStateTracker::PRESSED || keyboardTracker->IsKeyPressed(Keyboard::Up))
		tillAllocked = true;

	bool allLocked = true;

	do{
		allLocked = true;
		for (UINT i = 0; i < mTetrominos.size(); i++){
			mTetrominos[i]->update(gamePadTracker, keyboardTracker, keyboardState);
			if (!mSplitted){
				allLocked = false;
				if (mTetrominos[i]->isLocked()){
					split(mTetrominos[i]);
					break;
				}
			}
			else{
				allLocked &= mTetrominos[i]->isLocked();
				if (mTetrominos[i]->isLocked()){
					mTetrominos[i]->placeOnField();
					delete mTetrominos[i];
					mTetrominos.erase(mTetrominos.begin() + i);
					i--;
				}
			}
		}
	} while (tillAllocked && !allLocked);

	if (!mSplitted){
		//TODO: Input handling
		if (gamePadTracker->x == GamePad::ButtonStateTracker::PRESSED
			|| keyboardTracker->IsKeyPressed(Keyboard::A))
			rotate(ERotation::COUNTER_CLOCKWISE);
		else if (gamePadTracker->y == GamePad::ButtonStateTracker::PRESSED
			|| keyboardTracker->IsKeyPressed(Keyboard::D))
			rotate(ERotation::CLOCKWISE);

		if (gamePadTracker->dpadLeft == GamePad::ButtonStateTracker::PRESSED
			|| keyboardTracker->IsKeyPressed(Keyboard::Left))
			move(DirectX::SimpleMath::Vector2(-1, 0));
		if (gamePadTracker->dpadRight == GamePad::ButtonStateTracker::PRESSED
			|| keyboardTracker->IsKeyPressed(Keyboard::Right))
			move(DirectX::SimpleMath::Vector2(1, 0));
	}
	else if(allLocked){
		//TODO: Blöcke auf das Spielfeld überschreiben und neuen Tetromino spawnen.
		for (UINT i = 0; i < mTetrominos.size(); i++)
			mTetrominos[i]->placeOnField();

		mToBeDestroyed = true;

		Field::checkForLineClear();
	}
}

void TetrominoGroup::render(DirectX::SpriteBatch* spriteBatch){
	for (UINT i = 0; i < mTetrominos.size(); i++){
		mTetrominos[i]->render(spriteBatch);
	}
}

void TetrominoGroup::render(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2& screenPosition, float scale){
	for (UINT i = 0; i < mTetrominos.size(); i++){
		mTetrominos[i]->render(spriteBatch, screenPosition, scale);
	}
}

void TetrominoGroup::rotate(ERotation rotation){
	for (UINT i = 0; i < mTetrominos.size(); i++){
		mTetrominos[i]->rotate(rotation);
	}
}

bool TetrominoGroup::setPosition(DirectX::SimpleMath::Vector2& position){
	for (UINT i = 0; i < mTetrominos.size(); i++)
		mTetrominos[i]->setPosition(position);

	return true;
}

bool TetrominoGroup::move(DirectX::SimpleMath::Vector2& direction){
	bool successed = true;
	
	for (UINT i = 0; i < mTetrominos.size(); i++){
		successed &= mTetrominos[i]->move(direction);
	}
	return successed;
}

void TetrominoGroup::split(Tetromino* tetromino){
	std::vector<Tetromino*> newMinos;	//Liste der neuen Tetrominos.
	
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
					newMinos.push_back(tetromino->getPart(matrix, checked, width, height, x, y));
				}
			}

		}
	}

	for (UINT i = 0; i < mTetrominos.size(); i++)
		delete mTetrominos[i];

	mTetrominos = newMinos;
	mSplitted = true;

	delete[] checked;
}
