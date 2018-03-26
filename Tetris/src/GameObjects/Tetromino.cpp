#include <pch.h>
#include "Tetromino.h"

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
	mBaseMatrix = nullptr;
	mFieldMatrix = nullptr;

	mBaseMatrix.reset(new int[width * height]);
	memset(mBaseMatrix.get(), 0, width * height * sizeof(int));

	mFieldMatrix.reset(new int[width * height]);
	memset(mFieldMatrix.get(), 0, width * height * sizeof(int));

	m_originalMatrix.reset(new int[width * height]);
	memset(m_originalMatrix.get(), 0, width * height * sizeof(int));

	int n = 0;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			mBaseMatrix.get()[y * width + x] = 0;
			m_originalMatrix.get()[y * width + x] = 0;
			if (matrix[y * width + x]){
				n++;
				mBaseMatrix.get()[y * width + x] = n;
				m_originalMatrix.get()[y * width + x] = n;
			}
		}
	}


	if (color != sf::Color::White){
		
		for (int y = 0; y < height; y++){
			for (int x = 0; x < width; x++){
				
				if (matrix[y * width + x]){
					mBlocks.push_back(std::shared_ptr<Block>(new Block(m_field, m_positionGrid + sf::Vector2i(x, y), color)));
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

					mBlocks.push_back(std::shared_ptr<Block>(new Block(m_field, m_positionGrid + sf::Vector2i(x, y), color)));
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
}

void Tetromino::update(KeyboardStateTracker& keyboardTracker){
	m_triesToRotate = 0;
	if (!mLocked){
		mFrameCounter++;
		if (keyboardTracker.isKeyDown(sf::Keyboard::Down))
			mFrameCounter += 4;

		while (mFrameCounter >= 60.0f / m_field.getGravity()){
			mFrameCounter -= 60.0f / m_field.getGravity();
			if (!move(sf::Vector2i(0, 1))){
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

				sf::Vector2f off = sf::Vector2f(m_estimatedPositionGrid + sf::Vector2i(x, y)) * (float)BLOCK_SIZE;
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
	//Y coordinate of the lowest block in the matrix.
	int lowestY = y;
	
	if (!matrix)
		matrix = mBaseMatrix.get();

	bool newCheckedCreated = false;
	if (!checked){
		checked = new bool[width * height];
		memset(checked, 0, width * height * sizeof(bool));
		newCheckedCreated = true;
	}

	std::vector<sf::Vector2i> openList;
	std::vector<sf::Vector2i> closedList;

	Tetromino* newMino = new Tetromino(m_field, mMatrixWidth, mMatrixHeight);

	openList.push_back(sf::Vector2i(x, y));

	while (!openList.empty()){
		sf::Vector2i current = openList[0];
		closedList.push_back(current);
		openList.erase(openList.begin());
		int cX = (int)current.x;
		int cY = (int)current.y;

		if (cY > lowestY)
			lowestY = cY;

		UINT i = matrix[cY * width + cX];
		if (i <= mBlocks.size() && i > 0){
			Block b = *mBlocks[i - 1];
			b.setPosition(current);
			newMino->addBlock(b);
		}
		else{
			newMino->addBlock(Block(m_field, current));
		}

		for (int j = -1; j <= 1; j++){
			for (int i = -1; i <= 1; i++){
				if (!(i != 0 && j != 0) && !(i == 0 && j == 0)){
					
					sf::Vector2i checking = current + sf::Vector2i(i, j);

					int cX = (int)checking.x;
					int cY = (int)checking.y;

					if (cX < 0 || cX >= width || cY < 0 || cY >= height)
						continue;

					checked[cY * width + cX] = true;

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

						if (!onList){
							openList.push_back(checking);
						}

					}

				}
			}
		}
	}

	sf::Vector2i newPosition = m_positionGrid - sf::Vector2i(0, height - 1 - lowestY);
	newMino->setPosition(newPosition);

	if (newCheckedCreated)
		delete[] checked;

	return newMino;
}

int Tetromino::getBlockCount(){
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

	mBlocks.push_back(std::shared_ptr<Block>(new Block(block)));
	setFieldMatrix();
}

bool Tetromino::setPosition(sf::Vector2i& position){
	m_positionGrid = position;
	m_positionSreen = m_field.getScreenPosition() + sf::Vector2f(m_positionGrid) * (float)BLOCK_SIZE;


	bool collision = false;

	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){

			UINT i = mFieldMatrix.get()[y * mMatrixWidth + x];
			
			if (i > 0 && i <= mBlocks.size()){
				
				sf::Vector2i newPos = m_positionGrid + sf::Vector2i(x, y);
				collision |= !mBlocks[i - 1]->setPosition(newPos);
			}
		}
	}

	return !collision;
}

void Tetromino::rotate(ERotation rotation){
	m_triesToRotate++;

	if (m_triesToRotate > 10)
		return;

	int* newMat = new int[mMatrixWidth * mMatrixHeight];
	memset(newMat, 0, mMatrixWidth * mMatrixHeight * sizeof(int));

	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){
			int nX = 0;
			int nY = 0;

			if (rotation == ERotation::COUNTER_CLOCKWISE){
				nX = y;
				nY = mMatrixHeight - 1 - x; 
			}
			else if(rotation == ERotation::CLOCKWISE){
				nX = mMatrixWidth - 1 - y;	
				nY = x;						
			}

			newMat[nY * mMatrixWidth + nX] = mBaseMatrix.get()[y * mMatrixWidth + x];
		}
	}

	mBaseMatrix.reset(newMat);

	setFieldMatrix();
	
	sf::Vector2i oldPosition = m_positionGrid;

	if (!setPosition(oldPosition)){
		
		if (!setPosition(oldPosition + sf::Vector2i(1, 0))){
			
			if (!setPosition(oldPosition + sf::Vector2i(-1, 0))){
				
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
	for (unsigned x = 0; x < mMatrixWidth; x++){
		for (unsigned y = 0; y < mMatrixHeight; y++){
			if (mBaseMatrix.get()[y * mMatrixWidth + x])
				return sf::Vector2i(x, 0);
		}
	}
	return sf::Vector2i(-1, -1);
}

sf::Vector2i Tetromino::getBlockMostRight(){
	for (unsigned x = mMatrixWidth - 1; x >= 0; x--){
		for (unsigned y = 0; y < mMatrixHeight; y++){
			if (mBaseMatrix.get()[y * mMatrixWidth + x])
				return sf::Vector2i(x, 0);
		}
	}

	return sf::Vector2i(-1, -1);
}

bool Tetromino::move(sf::Vector2i& direction){
	sf::Vector2i oldPosition = m_positionGrid;

	bool collision = false;

	if (!setPosition(oldPosition + direction)){
		setPosition(oldPosition);
		collision = true;
	}

	return !collision;
}

void Tetromino::setFieldMatrix(){
	mFieldMatrix.reset(new int[mMatrixWidth * mMatrixHeight]);
	
	for (unsigned y = 0; y < mMatrixHeight; y++){
		for (unsigned x = 0; x < mMatrixWidth; x++){
			mFieldMatrix.get()[y * mMatrixWidth + x] = mBaseMatrix.get()[y * mMatrixWidth + x];
		}
	}

	int n = 0;

	while (n == 0){
		
		for (unsigned x = 0; x < mMatrixWidth; x++){
			if (mFieldMatrix.get()[(mMatrixHeight - 1) * mMatrixWidth + x] > 0)
				n++;
		}

		if (n) break;

		for (unsigned y = mMatrixHeight - 1; y > 0; y--){
			for (unsigned x = 0; x < mMatrixWidth; x++){
				mFieldMatrix.get()[y * mMatrixWidth + x] = mFieldMatrix.get()[(y - 1) * mMatrixWidth + x];
			}
		}

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
