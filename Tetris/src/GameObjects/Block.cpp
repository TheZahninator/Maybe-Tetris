#include <pch.h>
#include "Block.h"

Block::Block(sf::Vector2i& position, sf::Color& tint) :
	mTint(tint){

	setPosition(position);

	m_sprite = sf::Sprite(TextureManager::getTexture(TEX_BLOCK_WHITE));
}

void Block::render(sf::RenderWindow* window){
	if (mPositionGrid.y >= 0){
		m_sprite.setPosition((float)mPositionScreen.x, (float)mPositionScreen.y);
		m_sprite.setColor(mTint);

		window->draw(m_sprite);
	}
}

void Block::render(sf::RenderWindow* window, sf::Vector2i& screenPosition, float scale, sf::Color tint){
	m_sprite.setPosition(screenPosition.x, screenPosition.y);
	m_sprite.setColor(tint);
	m_sprite.setScale(scale, scale);

	window->draw(m_sprite);
}

bool Block::setPosition(sf::Vector2i& position){
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

sf::Vector2i& Block::getPosition(){
	return mPositionGrid;
}

sf::Color& Block::getTint(){
	return mTint;
}
