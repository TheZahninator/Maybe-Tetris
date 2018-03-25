#pragma once

#include "../Graphics/TextureManager.h"
#include "Field.h"

#define BLOCK_SIZE 16	//Blockgröße in Pixeln

class Block{
public:
	Block(Field& field, sf::Vector2i& position, sf::Color& tint = BLOCK_COLOR_SINGLE);

	void render(sf::RenderWindow* window);	//Zeichnet die Textur
	void render(sf::RenderWindow* window, sf::Vector2f& screenPosition, float scale = 1.0f, sf::Color tint = sf::Color::White); //Zeichnet die textur an einer bestimmten Stelle auf dem Bildschirm

	bool setPosition(sf::Vector2i& position);	//Setzt die Spielfeldposition und gibt zurück, ob es zu einer Kollision kommt.

	sf::Vector2i& getPosition();	//Gibt die Spielfeldposition zurück
	sf::Color& getTint();		//Gibt die Färbung der Textur zurück

private:
	sf::Vector2i mPositionGrid;	//Position auf dem Spielfeld
	sf::Vector2f mPositionScreen;	//Position auf dem Bildschirm
	sf::Color mTint;		//Färbung der Textur
	
	sf::Sprite m_sprite;

	Field& m_field;
};