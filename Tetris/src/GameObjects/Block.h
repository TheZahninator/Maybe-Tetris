#pragma once

#include "../Graphics/TextureManager.h"
#include "Field.h"

#define BLOCK_SIZE 16 //Block size in pixels
class Field;

class Block{
public:
	Block(Field& field, sf::Vector2i& position, sf::Color& tint = BLOCK_COLOR_SINGLE);

	void render(sf::RenderWindow* window);	
	void render(sf::RenderWindow* window, sf::Vector2f& screenPosition, float scale = 1.0f, sf::Color tint = sf::Color::White);

	bool setPosition(sf::Vector2i& position);

	sf::Vector2i& getPosition();
	sf::Color& getTint();

private:
	sf::Vector2i mPositionGrid;
	sf::Vector2f mPositionScreen;
	sf::Color mTint;
	
	sf::Sprite m_sprite;

	Field& m_field;
};