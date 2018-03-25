#include <pch.h>
#include "Block.h"

Block::Block(Field& field, sf::Vector2i& position, sf::Color& tint) :
mTint(tint), m_field(field)
{

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

void Block::render(sf::RenderWindow* window, sf::Vector2f& screenPosition, float scale, sf::Color tint){
	m_sprite.setPosition(screenPosition);
	m_sprite.setColor(tint);
	m_sprite.setScale(scale, scale);

	window->draw(m_sprite);
}

bool Block::setPosition(sf::Vector2i& position){
	bool collision = false;
	
	int nX = (int)position.x;
	int nY = (int)position.y;
	
	if (nX < 0 || nX > (int)m_field.getWidth() - 1 || nY > (int)m_field.getHeight() - 1)
		collision = true;

	if (nY >= 0 && nX >= 0)
		if (m_field.getGrid()[nY * m_field.getWidth() + nX])
			collision = true;

	mPositionGrid = position;
	mPositionScreen = m_field.getScreenPosition() + sf::Vector2f(mPositionGrid) * (float)BLOCK_SIZE;

	return !collision;
}

sf::Vector2i& Block::getPosition(){
	return mPositionGrid;
}

sf::Color& Block::getTint(){
	return mTint;
}
