#include <pch.h>
#include "Block.h"

Block::Block(DirectX::SimpleMath::Vector2& position, DirectX::SimpleMath::Vector4& tint) :
	mTint(tint){

	setPosition(position);
}

void Block::render(DirectX::SpriteBatch* spriteBatch){
	if (mPositionGrid.y >= 0)
		TextureManager::getTexture(TEX_BLOCK_WHITE)->draw(spriteBatch, mPositionScreen, mTint);
}

void Block::render(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2& screenPosition, float scale, DirectX::SimpleMath::Vector4 tint){
	TextureManager::getTexture(TEX_BLOCK_WHITE)->draw(spriteBatch, screenPosition, mTint * tint, 0.0f, DirectX::SimpleMath::Vector2(0, 0), scale);
}

bool Block::setPosition(DirectX::SimpleMath::Vector2& position){
	bool collision = false;
	
	int nX = (int)position.x;
	int nY = (int)position.y;
	
	if (nX < 0 || nX > (int)Field::getWidth() - 1 || nY > (int)Field::getHeight() - 1)
		collision = true;

	if (nY >= 0 && nX >= 0)
		if (Field::getGrid()[nY * Field::getWidth() + nX])
			collision = true;

	mPositionGrid = position;
	mPositionScreen = Field::getScreenPosition() + mPositionGrid * BLOCK_SIZE;

	return !collision;
}

DirectX::SimpleMath::Vector2& Block::getPosition(){
	return mPositionGrid;
}

DirectX::SimpleMath::Vector4& Block::getTint(){
	return mTint;
}
