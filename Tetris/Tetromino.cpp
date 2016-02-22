#include "pch.h"
#include "Tetromino.h"

using namespace DirectX::SimpleMath;

//Setzt nur die nötigsten Variablen auf ihre Initialisierungswerte.
Tetromino::Tetromino() : mMatrixWidth(0), mMatrixHeight(0), mLocked(false), mCollisionAtSpawn(false){
	mBaseMatrix = nullptr;
	mFieldMatrix = nullptr;
}

Tetromino::Tetromino(int width, int height) : mMatrixWidth(width), mMatrixHeight(height), mFrameCounter(0.0f), mLocked(false), mCollisionAtSpawn(false){
	mBaseMatrix = nullptr;
	mFieldMatrix = nullptr;

	mBaseMatrix = new int[width * height];
	memset(mBaseMatrix, 0, width * height * sizeof(int));

	mFieldMatrix = new int[width * height];
	memset(mFieldMatrix, 0, width * height * sizeof(int));
}

Tetromino::Tetromino(Vector2& position, bool* matrix, int width, int height, Vector4& color) : mPositionGrid(position), mMatrixWidth(width), mMatrixHeight(height), mFrameCounter(0.0f), mLocked(false), mCollisionAtSpawn(false){
	//Setzt beide Matrizen auf nullptr.
	mBaseMatrix = nullptr;
	mFieldMatrix = nullptr;

	//Erstellt die Base Matrix und füllt sie mit Nullen.
	mBaseMatrix = new int[width * height];
	memset(mBaseMatrix, 0, mMatrixWidth * mMatrixHeight * sizeof(int));
	
	//Erstellt die Feld Matrix und füllt sie mit Nullen.
	mFieldMatrix = new int[width * height];
	memset(mFieldMatrix, 0, mMatrixWidth * mMatrixHeight * sizeof(int));

	//Iteriert durch alle Felder der mitgegebenen Matrix und zählt dabei wie viele Blöcke enthalten sind.
	int n = 0;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			mBaseMatrix[y * width + x] = 0;
			if (matrix[y * width + x]){
				//Wenn ein Block gefunden wurde, wird n um eins erhöht.
				n++;
				//Das entsprechende Feld in der Base Matrix wird auf den Wert n gesetzt.
				mBaseMatrix[y * width + x] = n;
			}
		}
	}


	if (color != DirectX::Colors::White.v){
		
		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){
				
				if (matrix[y * width + x]){
					mBlocks.push_back(std::shared_ptr<Block>(new Block(mPositionGrid + Vector2((float)x, (float)y), color)));
				}

			}
		}
	
	}
	else{

		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){

				if (matrix[y * width + x]){
					Vector4 color = BLOCK_COLOR_SINGLE;

					Tetromino* tempMino = getPart(nullptr, nullptr, width, height, x, y);
					int n = tempMino->getBlockCount();
					delete tempMino;

					switch (n){
					case 1:
						color = BLOCK_COLOR_SINGLE;
						break;
					case 2:
						color = BLOCK_COLOR_DOUBLE;
						break;
					case 3:
						color = BLOCK_COLOR_TRIPLE;
						break;
					case 4:
						color = BLOCK_COLOR_QUADRUPLE;
						break;
					}

					mBlocks.push_back(std::shared_ptr<Block>(new Block(mPositionGrid + Vector2((float)x, (float)y), color)));
				}

			}
		}

	}

	setFieldMatrix();
	mCollisionAtSpawn = !setPosition(position);
}

Tetromino::~Tetromino(){
	//Wenn eine Matrix erstellt wurde wird sie gelöscht.
	if (mBaseMatrix){
		delete[] mBaseMatrix;
		mBaseMatrix = nullptr;
	}
	//Hier genauso
	if (mFieldMatrix){
		delete[] mFieldMatrix;
		mFieldMatrix = nullptr;
	}
}

void Tetromino::update(DirectX::GamePad::ButtonStateTracker* gamePadTracker, DirectX::Keyboard::KeyboardStateTracker* keyboardTracker, DirectX::Keyboard::State* keyboardState){
	if (!mLocked){
		//Zählt einen Frame.
		mFrameCounter++;
		//Wenn nach unten gehalten wird, werden nochmal 4 Frames hinzugezählt.
		if (gamePadTracker->dpadDown == GamePad::ButtonStateTracker::HELD || keyboardState->Down)
			mFrameCounter += 4;

		//Solange mehr Frames gezählt werden als pro Reihe gewartet werden soll, fällt der Stein um eine Reihe.
		while (mFrameCounter >= 60.0f / Field::getGravity()){
			//Die zu wartenden Frames werden wieder abgezogen.
			mFrameCounter -= 60.0f / Field::getGravity();
			//Wenn eine Kollision stattfindet, darf er nicht mehr fallen.
			if (!move(Vector2(0.0f, 1.0f))){
				mLocked = true;
			}
		}
	}
}

void Tetromino::render(DirectX::SpriteBatch* spriteBatch){
	for (UINT i = 0; i < mBlocks.size(); i++){
		mBlocks[i]->render(spriteBatch);
	}
}

void Tetromino::render(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2& screenPosition, float scale){
	for (int y = 0; y < mMatrixHeight; y++){
		for (int x = 0; x < mMatrixWidth; x++){
			int i = mBaseMatrix[y * mMatrixWidth + x];
			if (i){
				mBlocks[i-1]->render(spriteBatch, screenPosition + Vector2(x, y) * (BLOCK_SIZE * scale), scale);
			}
		}
	}
}

Tetromino* Tetromino::getPart(int* matrix, bool* checked, int width, int height, int x, int y){
	//Y Koordinate des untersten Blocks in der Matrix.
	int lowestY = y;
	
	//Wenn keine Matrix mitgegeben wurde wird die eigene genommen.
	if (!matrix)
		matrix = mBaseMatrix;

	//Wenn keine Überprüfungs Matrix mitgegeben wurde, wird eine neue erstellt.
	bool newCheckedCreated = false;
	if (!checked){
		checked = new bool[width * height];
		memset(checked, 0, width * height * sizeof(bool));
		newCheckedCreated = true;
	}

	//Liste um zu speichern, welche Felder noch überprüft werden müssen.
	std::vector<Vector2> openList;
	//Liste um zu speichern, welche Felder nicht mehr überprüft werden dürfen.
	std::vector<Vector2> closedList;

	//Speicherplatz für den neuen Tetromino.
	//Ich deklariere ihn als pointer damit er bei Funktionsende nicht gelöscht wird, was einen Fehler auslösen würde.
	Tetromino* newMino = new Tetromino(mMatrixWidth, mMatrixHeight);

	//Packt das erste Feld in die Liste zum überprüfen.
	openList.push_back(Vector2((float)x, (float)y));

	//Läuft solange Felder zum Überprüfen in der Liste sind.
	while (!openList.empty()){
		//Entfernt das erste Feld aus der Liste und tut es in die Andere.
		Vector2 current = openList[0];
		closedList.push_back(current);
		openList.erase(openList.begin());

		//Hilfsvariablen da man mit Vectoren nicht auf Arrays zugreifen kann.
		int cX = (int)current.x;
		int cY = (int)current.y;

		if (cY > lowestY)
			lowestY = cY;

		//Farbe die der neue Block im neuen Tetromino bekommt.
		//DirectX::SimpleMath::Vector4 color = BLOCK_COLOR_SINGLE;

		//Überprüft ob der Block bereits vorhanden ist. Wenn ja, wird er dem Tetromino hinzugefügt.
		UINT i = matrix[cY * width + cX];
		if (i <= mBlocks.size() && i > 0){
			Block b = *mBlocks[i - 1];
			b.setPosition(current);
			newMino->addBlock(b);
		}
		else{
			//Fügt dem neuen Tetromino einen neuen Block hinzu.
			newMino->addBlock(Block(current));
		}

		//Überprüft nacheinander jedes Feld das direkt an der momentanen Position anliegt.
		for (int j = -1; j <= 1; j++){
			for (int i = -1; i <= 1; i++){
				if (!(i != 0 && j != 0) && !(i == 0 && j == 0)){
					
					//Momentan überprüfte Position
					Vector2 checking = current + Vector2((float)i, (float)j);

					//Helfer um den Index im Array zu berechnen.
					int cX = (int)checking.x;
					int cY = (int)checking.y;

					//Wenn das Feld außerhalb der Matrix liegt, wird mit dem nächsten fortgefahren.
					if (cX < 0 || cX >= width || cY < 0 || cY >= height)
						continue;

					//Das aktuelle Feld wird als überprüft markiert.
					checked[cY * width + cX] = true;

					//Wenn in der Matrix auf diesem Feld ein Block liegtwird überprüft, ob dieses Feld bereits in eine der beiden Listen vorhanden ist.
					if (matrix[cY * width + cX]){

						bool onList = false;
						for (auto vec : closedList){
							if (vec == checking)
								onList = true;
						}
						for (auto vec : openList){
							if (vec == checking)
								onList = true;
						}

						//Wenn das Feld in keiner der Listen vorhanden ist, wird es der openList hinzugefügt.
						if (!onList){
							openList.push_back(checking);
						}

					}

				}
			}
		}
	}

	Vector2 newPosition = mPositionGrid - Vector2(0.0f, (float)(height - 1 - lowestY));
	newMino->setPosition(newPosition);

	if (newCheckedCreated)
		delete[] checked;

	return newMino;
}

int Tetromino::getBlockCount(){
	//Gibt die Anzahl der Blöcke dieses Tetrominos zurück.
	return mBlocks.size();
}

void Tetromino::addBlock(Block& block){
	int x = (int)block.getPosition().x;
	int y = (int)block.getPosition().y;
	
	if (mBaseMatrix){
		int i = mBaseMatrix[y * mMatrixWidth + x];
		if (i > 0){
			mBlocks[i].reset(new Block(block));
			return;
		}
		else{
			mBaseMatrix[y * mMatrixWidth + x] = getBlockCount() + 1;
		}
	}

	//Fügt den Block der Liste hinzu.
	mBlocks.push_back(std::shared_ptr<Block>(new Block(block)));
	setFieldMatrix();
}

bool Tetromino::setPosition(Vector2& position){
	//Aktualisiert die Positionen.
	mPositionGrid = position;
	mPositionSreen = Field::getScreenPosition() + mPositionGrid * BLOCK_SIZE;


	//Variable um zu speichern, ob eine Kollision stattfindet.
	bool collision = false;

	//Iteriert durch alle Felder der Matrix.
	for (int y = 0; y < mMatrixHeight; y++){
		for (int x = 0; x < mMatrixWidth; x++){

			//Überprüft den Wert an der Stelle.
			UINT i = mFieldMatrix[y * mMatrixWidth + x];
			
			if (i > 0 && i <= mBlocks.size()){
				
				//Wenn ein Block an der Stelle ist, wird seine Position aktualisiert.
				Vector2 newPos = mPositionGrid + Vector2((float)x, (float)y);
				collision |= !mBlocks[i - 1]->setPosition(newPos);
			}
		}
	}

	//Gibt "true" zurück, wenn keine Kollision stattfindet.
	return !collision;
}

void Tetromino::rotate(ERotation rotation){
	
	//Eine neue Matrix mit der den Dimensionen der alten Matrix wird angelegt.
	int* newMat = new int[mMatrixWidth * mMatrixHeight];
	memset(newMat, 0, mMatrixWidth * mMatrixHeight * sizeof(int));

	//Iteriert durch jedes Feld der Matrix durch und füllt die neue Matrix mit den entsprechenden Werten an den neuen Stellen.
	for (int y = 0; y < mMatrixHeight; y++){
		for (int x = 0; x < mMatrixWidth; x++){
			//Variablen für die neue Position des Feldes.
			int nX = 0;
			int nY = 0;

			//Überprüfung ob im oder gegen den Uhrzeigersinn gedreht wird.
			if (rotation == ERotation::COUNTER_CLOCKWISE){
				//Wenn gegen den Uhrzeigersinn wird die neue Position entsprechend gesetzt.
				nX = y;						//Neue x-Position
				nY = mMatrixHeight - 1 - x; //Neue y-Position
			}
			else if(rotation == ERotation::CLOCKWISE){
				//Wenn im Uhrzeigersinn wird die neie Position entsprechend gesetzt.
				nX = mMatrixWidth - 1 - y;	//Neue x-Position
				nY = x;						//Neue y-Position
			}

			//Füllt das Feld in der neuen Matrix mit dem Wert des entsprecheden Feldes der alten Matrix.
			newMat[nY * mMatrixWidth + nX] = mBaseMatrix[y * mMatrixWidth + x];
		}
	}
	//Löscht die alte Matrix.
	delete[] mBaseMatrix;
	mBaseMatrix = nullptr;

	//Überträgt die Speicheradresse der neuen Matrix.
	mBaseMatrix = newMat;

	//Die auf dem Spielfeld benutzte Matrix wird aktualisiert.
	setFieldMatrix();
	
	Vector2 oldPosition = mPositionGrid;

	//Es werden die Blöcke auf ihre neuen Positionen gesetzt und überprüft, ob eine Kollision stattfindet
	if (!setPosition(oldPosition)){
		
		//Wenn eine Kollision mit einem anderen Block stattfindet, wird es ein Feld weiter rechts versucht
		if (!setPosition(oldPosition + Vector2( 1.0f, 0.0f))){
			
			//Wenn wieder eine Kollision stattfindet, wird es stattdessen ein Feld links versucht
			if (!setPosition(oldPosition + Vector2(-1.0f, 0.0f))){
				
				//Wenn wieder eine Kollision stattfindet, wird die Rotation rückgängig gemacht
				mPositionGrid = oldPosition;

				if (rotation == ERotation::CLOCKWISE)
					rotate(ERotation::COUNTER_CLOCKWISE);
				else
					rotate(ERotation::CLOCKWISE);
			}
		}
	}
		
}

DirectX::SimpleMath::Vector2 Tetromino::getBlockMostLeft(){
	//Geht von links angefangen jede Spalte durch.
	for (int x = 0; x < mMatrixWidth; x++){
		for (int y = 0; y < mMatrixHeight; y++){
			//Wenn ein Block gefunden wurde wird die x position zurück gegeben.
			if (mBaseMatrix[y * mMatrixWidth + x])
				return Vector2((float)x, 0.0f);
		}
	}
	//Wenn kein Block gefunden wurde wird ein Vector2 mit Fehler Koordinaten zurückgegeben.
	return Vector2(-1, -1);
}

DirectX::SimpleMath::Vector2 Tetromino::getBlockMostRight(){
	//Geht von links angefangen jede Spalte durch.
	for (int x = mMatrixWidth - 1; x >= 0; x--){
		for (int y = 0; y < mMatrixHeight; y++){
			//Wenn ein Block gefunden wurde wird die x position zurück gegeben.
			if (mBaseMatrix[y * mMatrixWidth + x])
				return Vector2((float)x, 0.0f);
		}
	}
	//Wenn kein Block gefunden wurde wird ein Vector2 mit Fehler Koordinaten zurückgegeben.
	return Vector2(-1, -1);
}

bool Tetromino::move(Vector2& direction){
	Vector2 oldPosition = mPositionGrid;

	bool collision = false;

	//Wenn die neue Position zur Kollision führt, wird sie wieder auf den vorherigen Wert gesetzt.
	if (!setPosition(oldPosition + direction)){
		setPosition(oldPosition);
		collision = true;
	}

	return !collision;
}

void Tetromino::setFieldMatrix(){
	if (mFieldMatrix){
		delete[] mFieldMatrix;
		mFieldMatrix = nullptr;
	}

	mFieldMatrix = new int[mMatrixWidth * mMatrixHeight];
	memset(mFieldMatrix, 0, mMatrixWidth * mMatrixHeight + sizeof(int));
	
	//Alle daten von der Basismatrix werden auf die Feldmatrix kopiert.
	for (int y = 0; y < mMatrixHeight; y++){
		for (int x = 0; x < mMatrixWidth; x++){
			mFieldMatrix[y * mMatrixWidth + x] = mBaseMatrix[y * mMatrixWidth + x];
		}
	}

	//Zähler Variable
	int n = 0;

	//Solange n = 0 ist soll die matrix pro schleifen durchlauf um ein Feld nach unten verschoben werden.
	while (n == 0){
		
		//Schleife um die Anzahl der Blöcke in der untersten Reihe der Matrix zu zählen.
		for (int x = 0; x < mMatrixWidth; x++){
			if (mFieldMatrix[(mMatrixHeight - 1) * mMatrixWidth + x] > 0)
				n++;
		}

		//Wenn Blöcke in der untersten Reihe sind: aufhören zu verschieben.
		if (n) break;

		//Jede Reihe wird einzeln nach unten verschoben
		for (int y = mMatrixHeight - 1; y > 0; y--){
			for (int x = 0; x < mMatrixWidth; x++){
				mFieldMatrix[y * mMatrixWidth + x] = mFieldMatrix[(y - 1) * mMatrixWidth + x];
			}
		}

		//Oberste Reihe wird gelöscht
		for (int x = 0; x < mMatrixWidth; x++){
			mFieldMatrix[x] = 0;
		}
	}
}

void Tetromino::placeOnField(){
	for (UINT i = 0; i < mBlocks.size(); i++){
		Field::addBlock(new Block(*mBlocks[i]));
	}
}
