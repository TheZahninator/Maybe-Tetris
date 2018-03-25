#include <pch.h>
#include "Tetromino.h"

//Setzt nur die nötigsten Variablen auf ihre Initialisierungswerte.
Tetromino::Tetromino(Field& field) : mMatrixWidth(0), mMatrixHeight(0), mLocked(false), mCollisionAtSpawn(false), m_field(field)
{
	m_triesToRotate = 0;

	m_estimatedPositionGrid = m_positionGrid;
	m_estimatedTouchingBlocks = 0;
	m_touchingBlocks = 0;
}

Tetromino::Tetromino(Field& field, int width, int height) : mMatrixWidth(width), mMatrixHeight(height), mFrameCounter(0.0f), mLocked(false), mCollisionAtSpawn(false), m_field(field)
{
	
	mBaseMatrix.reset(new int[width * height]);
	memset(mBaseMatrix.get(), 0, width * height * sizeof(int));

	mFieldMatrix.reset(new int[width * height]);
	memset(mFieldMatrix.get(), 0, width * height * sizeof(int));

	m_originalMatrix.reset(new int[width * height]);
	memset(m_originalMatrix.get(), 0, width * height * sizeof(int));

	m_triesToRotate = 0;

	m_estimatedPositionGrid = m_positionGrid;
	m_estimatedTouchingBlocks = 0;
	m_touchingBlocks = 0;
}

Tetromino::Tetromino(Field& field, sf::Vector2i& position, bool* matrix, int width, int height, sf::Color& color) : m_positionGrid(position), mMatrixWidth(width), mMatrixHeight(height), mFrameCounter(0.0f), mLocked(false), mCollisionAtSpawn(false), m_field(field)
{
	//Setzt beide Matrizen auf nullptr.
	mBaseMatrix = nullptr;
	mFieldMatrix = nullptr;

	//Erstellt die Base Matrix und füllt sie mit Nullen.
	mBaseMatrix.reset(new int[width * height]);
	memset(mBaseMatrix.get(), 0, width * height * sizeof(int));

	mFieldMatrix.reset(new int[width * height]);
	memset(mFieldMatrix.get(), 0, width * height * sizeof(int));

	m_originalMatrix.reset(new int[width * height]);
	memset(m_originalMatrix.get(), 0, width * height * sizeof(int));

	//Iteriert durch alle Felder der mitgegebenen Matrix und zählt dabei wie viele Blöcke enthalten sind.
	int n = 0;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			mBaseMatrix.get()[y * width + x] = 0;
			m_originalMatrix.get()[y * width + x] = 0;
			if (matrix[y * width + x]){
				//Wenn ein Block gefunden wurde, wird n um eins erhöht.
				n++;
				//Das entsprechende Feld in der Base Matrix wird auf den Wert n gesetzt.
				mBaseMatrix.get()[y * width + x] = n;
				m_originalMatrix.get()[y * width + x] = n;
			}
		}
	}


	if (color != sf::Color::White){
		
		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){
				
				if (matrix[y * width + x]){
					mBlocks.push_back(std::shared_ptr<Block>(new Block(m_field, m_positionGrid + sf::Vector2i((float)x, (float)y), color)));
				}

			}
		}
	
	}
	else{

		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){

				if (matrix[y * width + x]){
					sf::Color color = BLOCK_COLOR_SINGLE;

					std::unique_ptr<Tetromino> tempMino(getPart(nullptr, nullptr, width, height, x, y));
					int n = tempMino->getBlockCount();
					tempMino.reset();

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

					mBlocks.push_back(std::shared_ptr<Block>(new Block(m_field, m_positionGrid + sf::Vector2i((float)x, (float)y), color)));
				}

			}
		}

	}

	setFieldMatrix();
	mCollisionAtSpawn = !setPosition(position);
	m_triesToRotate = 0;

	m_estimatedPositionGrid = m_positionGrid;
	m_estimatedTouchingBlocks = 0;
	m_touchingBlocks = 0;
}

Tetromino::~Tetromino(){
	/*//Wenn eine Matrix erstellt wurde wird sie gelöscht.
	if (mBaseMatrix){
		//delete[] mBaseMatrix;
		mBaseMatrix = nullptr;
	}
	//Hier genauso
	if (mFieldMatrix){
		//delete[] mFieldMatrix;
		mFieldMatrix = nullptr;
	}*/
}

void Tetromino::update(KeyboardStateTracker* keyboardTracker){
	m_triesToRotate = 0;
	if (!mLocked){
		//Zählt einen Frame.
		mFrameCounter++;
		//Wenn nach unten gehalten wird, werden nochmal 4 Frames hinzugezählt.
		if (keyboardTracker->isKeyDown(sf::Keyboard::Down))
			mFrameCounter += 4;

		//Solange mehr Frames gezählt werden als pro Reihe gewartet werden soll, fällt der Stein um eine Reihe.
		while (mFrameCounter >= 60.0f / m_field.getGravity()){
			//Die zu wartenden Frames werden wieder abgezogen.
			mFrameCounter -= 60.0f / m_field.getGravity();
			//Wenn eine Kollision stattfindet, darf er nicht mehr fallen.
			if (!move(sf::Vector2i(0.0f, 1.0f))){
				mLocked = true;
			}
		}
	}

	//Calculate the estimated grid position
	sf::Vector2i oldPos = m_positionGrid;

	bool collision = false;
	while (!collision){
		sf::Vector2i newPos = m_positionGrid;
		newPos.y++;

		collision = !setPosition(newPos);
	}

	m_estimatedPositionGrid = m_positionGrid;
	m_estimatedPositionGrid.y--;

	//Calculate estimated touching blocks
	calcTouchingBlocks(m_estimatedTouchingBlocks);

	setPosition(oldPos);

	calcTouchingBlocks(m_touchingBlocks);
}

void Tetromino::calcTouchingBlocks(UINT& result){
	result = 0;

	for (unsigned x = 0; x < mMatrixWidth; x++){
		for (unsigned y = 0; y < mMatrixHeight; y++){
			int i = mFieldMatrix.get()[y * mMatrixWidth + x];
			if (i){

				//Check all sides of the block
				for (int xx = -1; xx <= 1; xx++){
					for (int yy = -1; yy <= 1; yy++){

						//Don't check diagonals
						if (!(xx != 0 && yy != 0) && !(xx == 0 && yy == 0))

						//Check whether the checked block is in the own matrix
						if ((x + xx) >= 0 && (x + xx) < mMatrixWidth && (y + yy) >= 0 && (y + yy) < mMatrixHeight){
							if (mFieldMatrix.get()[(y + yy) * mMatrixWidth + (x + xx)]){	//If the checked block is its own block, skip it
								continue;
							}
							else{	//Else continue

								unsigned cx = static_cast<unsigned>(m_estimatedPositionGrid.x + x + xx);
								unsigned cy = static_cast<unsigned>(m_estimatedPositionGrid.y + y + yy);

								if (m_field.getGrid(cx, cy)){
									result++;
								}

							}
						}
						else{	//Else continue

							unsigned cx = static_cast<unsigned>(m_estimatedPositionGrid.x + x + xx);
							unsigned cy = static_cast<unsigned>(m_estimatedPositionGrid.y + y + yy);

							if (m_field.getGrid(cx, cy)){
								result++;
							}

						}

					}
				}

			}

		}
	}
}

void Tetromino::render(sf::RenderWindow* window){
	for (UINT i = 0; i < mBlocks.size(); i++){
		mBlocks[i]->render(window);
	}

	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){
			int i = mFieldMatrix.get()[y * mMatrixWidth + x];
			if (i){

				sf::Vector2i off = (m_estimatedPositionGrid + sf::Vector2i(x, y)) * BLOCK_SIZE;
				sf::Vector2f ghostPos(off.x, off.y);
				ghostPos += m_field.getScreenPosition();

				mBlocks[i - 1]->render(window, ghostPos, 1.0f, mBlocks[i-1]->getTint() * COLOR_ALPHA_50);

			}
		}
	}
}

void Tetromino::render(sf::RenderWindow* window, const sf::Vector2f& screenPosition, float scale){
	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){
			int i = mBaseMatrix.get()[y * mMatrixWidth + x];
			if (i){
				mBlocks[i-1]->render(window, screenPosition + sf::Vector2f((float)x, (float)y) * (BLOCK_SIZE * scale), scale, mBlocks[i-1]->getTint());
			}
		}
	}
}

Tetromino* Tetromino::getPart(int* matrix, bool* checked, int width, int height, int x, int y){
	//Y Koordinate des untersten Blocks in der Matrix.
	int lowestY = y;
	
	//Wenn keine Matrix mitgegeben wurde wird die eigene genommen.
	if (!matrix)
		matrix = mBaseMatrix.get();

	//Wenn keine Überprüfungs Matrix mitgegeben wurde, wird eine neue erstellt.
	bool newCheckedCreated = false;
	if (!checked){
		checked = new bool[width * height];
		memset(checked, 0, width * height * sizeof(bool));
		newCheckedCreated = true;
	}

	//Liste um zu speichern, welche Felder noch überprüft werden müssen.
	std::vector<sf::Vector2i> openList;
	//Liste um zu speichern, welche Felder nicht mehr überprüft werden dürfen.
	std::vector<sf::Vector2i> closedList;

	//Speicherplatz für den neuen Tetromino.
	//Ich deklariere ihn als pointer damit er bei Funktionsende nicht gelöscht wird, was einen Fehler auslösen würde.
	Tetromino* newMino = new Tetromino(m_field, mMatrixWidth, mMatrixHeight);

	//Packt das erste Feld in die Liste zum überprüfen.
	openList.push_back(sf::Vector2i((float)x, (float)y));

	//Läuft solange Felder zum Überprüfen in der Liste sind.
	while (!openList.empty()){
		//Entfernt das erste Feld aus der Liste und tut es in die Andere.
		sf::Vector2i current = openList[0];
		closedList.push_back(current);
		openList.erase(openList.begin());

		//Hilfsvariablen da man mit Vectoren nicht auf Arrays zugreifen kann.
		int cX = (int)current.x;
		int cY = (int)current.y;

		if (cY > lowestY)
			lowestY = cY;

		//Farbe die der neue Block im neuen Tetromino bekommt.
		//sf::Color color = BLOCK_COLOR_SINGLE;

		//Überprüft ob der Block bereits vorhanden ist. Wenn ja, wird er dem Tetromino hinzugefügt.
		UINT i = matrix[cY * width + cX];
		if (i <= mBlocks.size() && i > 0){
			Block b = *mBlocks[i - 1];
			b.setPosition(current);
			newMino->addBlock(b);
		}
		else{
			//Fügt dem neuen Tetromino einen neuen Block hinzu.
			newMino->addBlock(Block(m_field, current));
		}

		//Überprüft nacheinander jedes Feld das direkt an der momentanen Position anliegt.
		for (int j = -1; j <= 1; j++){
			for (int i = -1; i <= 1; i++){
				if (!(i != 0 && j != 0) && !(i == 0 && j == 0)){
					
					//Momentan überprüfte Position
					sf::Vector2i checking = current + sf::Vector2i((float)i, (float)j);

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

	sf::Vector2i newPosition = m_positionGrid - sf::Vector2i(0.0f, (float)(height - 1 - lowestY));
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
		int i = mBaseMatrix.get()[y * mMatrixWidth + x];
		if (i > 0){
			mBlocks[i].reset(new Block(block));
			return;
		}
		else{
			mBaseMatrix.get()[y * mMatrixWidth + x] = getBlockCount() + 1;
		}
	}

	//Fügt den Block der Liste hinzu.
	mBlocks.push_back(std::shared_ptr<Block>(new Block(block)));
	setFieldMatrix();
}

bool Tetromino::setPosition(sf::Vector2i& position){
	//Aktualisiert die Positionen.
	m_positionGrid = position;
	m_positionSreen = m_field.getScreenPosition() + sf::Vector2f(m_positionGrid) * (float)BLOCK_SIZE;


	//Variable um zu speichern, ob eine Kollision stattfindet.
	bool collision = false;

	//Iteriert durch alle Felder der Matrix.
	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){

			//Überprüft den Wert an der Stelle.
			UINT i = mFieldMatrix.get()[y * mMatrixWidth + x];
			
			if (i > 0 && i <= mBlocks.size()){
				
				//Wenn ein Block an der Stelle ist, wird seine Position aktualisiert.
				sf::Vector2i newPos = m_positionGrid + sf::Vector2i((float)x, (float)y);
				collision |= !mBlocks[i - 1]->setPosition(newPos);
			}
		}
	}

	//Gibt "true" zurück, wenn keine Kollision stattfindet.
	return !collision;
}

void Tetromino::rotate(ERotation rotation){
	m_triesToRotate++;
	if (m_triesToRotate > 10)
		return;
	//Eine neue Matrix mit der den Dimensionen der alten Matrix wird angelegt.
	int* newMat = new int[mMatrixWidth * mMatrixHeight];
	memset(newMat, 0, mMatrixWidth * mMatrixHeight * sizeof(int));

	//Iteriert durch jedes Feld der Matrix durch und füllt die neue Matrix mit den entsprechenden Werten an den neuen Stellen.
	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){
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
			newMat[nY * mMatrixWidth + nX] = mBaseMatrix.get()[y * mMatrixWidth + x];
		}
	}
	//Löscht die alte Matrix.
	//delete[] mBaseMatrix;
	//mBaseMatrix = nullptr

	//Überträgt die Speicheradresse der neuen Matrix.
	mBaseMatrix.reset(newMat);

	//Die auf dem Spielfeld benutzte Matrix wird aktualisiert.
	setFieldMatrix();
	
	sf::Vector2i oldPosition = m_positionGrid;

	//Es werden die Blöcke auf ihre neuen Positionen gesetzt und überprüft, ob eine Kollision stattfindet
	if (!setPosition(oldPosition)){
		
		//Wenn eine Kollision mit einem anderen Block stattfindet, wird es ein Feld weiter rechts versucht
		if (!setPosition(oldPosition + sf::Vector2i( 1.0f, 0.0f))){
			
			//Wenn wieder eine Kollision stattfindet, wird es stattdessen ein Feld links versucht
			if (!setPosition(oldPosition + sf::Vector2i(-1.0f, 0.0f))){
				
				//Wenn wieder eine Kollision stattfindet, wird die Rotation rückgängig gemacht
				m_positionGrid = oldPosition;

				if (rotation == ERotation::CLOCKWISE)
					rotate(ERotation::COUNTER_CLOCKWISE);
				else
					rotate(ERotation::CLOCKWISE);
			}
		}
	}
		
}

sf::Vector2i Tetromino::getBlockMostLeft(){
	//Geht von links angefangen jede Spalte durch.
	for (unsigned x = 0; x < mMatrixWidth; x++){
		for (unsigned y = 0; y < mMatrixHeight; y++){
			//Wenn ein Block gefunden wurde wird die x position zurück gegeben.
			if (mBaseMatrix.get()[y * mMatrixWidth + x])
				return sf::Vector2i((float)x, 0.0f);
		}
	}
	//Wenn kein Block gefunden wurde wird ein sf::Vector2i mit Fehler Koordinaten zurückgegeben.
	return sf::Vector2i(-1, -1);
}

sf::Vector2i Tetromino::getBlockMostRight(){
	//Geht von links angefangen jede Spalte durch.
	for (unsigned x = mMatrixWidth - 1; x >= 0; x--){
		for (unsigned y = 0; y < mMatrixHeight; y++){
			//Wenn ein Block gefunden wurde wird die x position zurück gegeben.
			if (mBaseMatrix.get()[y * mMatrixWidth + x])
				return sf::Vector2i((float)x, 0.0f);
		}
	}
	//Wenn kein Block gefunden wurde wird ein sf::Vector2i mit Fehler Koordinaten zurückgegeben.
	return sf::Vector2i(-1, -1);
}

bool Tetromino::move(sf::Vector2i& direction){
	sf::Vector2i oldPosition = m_positionGrid;

	bool collision = false;

	//Wenn die neue Position zur Kollision führt, wird sie wieder auf den vorherigen Wert gesetzt.
	if (!setPosition(oldPosition + direction)){
		setPosition(oldPosition);
		collision = true;
	}

	return !collision;
}

void Tetromino::setFieldMatrix(){
	//if (mFieldMatrix){
	//	delete[] mFieldMatrix;
	//	mFieldMatrix = nullptr;
	//}

	mFieldMatrix.reset(new int[mMatrixWidth * mMatrixHeight]);
	//memset(mFieldMatrix.get(), 0, mMatrixWidth * mMatrixHeight * sizeof(int));
	
	//Alle daten von der Basismatrix werden auf die Feldmatrix kopiert.
	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){
			mFieldMatrix.get()[y * mMatrixWidth + x] = mBaseMatrix.get()[y * mMatrixWidth + x];
		}
	}

	//Zähler Variable
	int n = 0;

	//Solange n = 0 ist soll die matrix pro schleifen durchlauf um ein Feld nach unten verschoben werden.
	while (n == 0){
		
		//Schleife um die Anzahl der Blöcke in der untersten Reihe der Matrix zu zählen.
		for (unsigned x = 0; x < mMatrixWidth; x++){
			if (mFieldMatrix.get()[(mMatrixHeight - 1) * mMatrixWidth + x] > 0)
				n++;
		}

		//Wenn Blöcke in der untersten Reihe sind: aufhören zu verschieben.
		if (n) break;

		//Jede Reihe wird einzeln nach unten verschoben
		for (unsigned y = mMatrixHeight - 1; y > 0; y--){
			for (unsigned x = 0; x < mMatrixWidth; x++){
				mFieldMatrix.get()[y * mMatrixWidth + x] = mFieldMatrix.get()[(y - 1) * mMatrixWidth + x];
			}
		}

		//Oberste Reihe wird gelöscht
		for (unsigned x = 0; x < mMatrixWidth; x++){
			mFieldMatrix.get()[x] = 0;
		}
	}
}

void Tetromino::placeOnField(){
	for (UINT i = 0; i < mBlocks.size(); i++){
		m_field.addBlock(mBlocks[i]);
	}
}
