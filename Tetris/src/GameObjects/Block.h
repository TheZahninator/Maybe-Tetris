#pragma once

#include "../Graphics/TextureManager.h"
#include "Field.h"

#define BLOCK_SIZE 16	//Blockgr��e in Pixeln

class Block{
public:
	Block(sf::Vector2i& position, sf::Color& tint = BLOCK_COLOR_SINGLE);

	void render(sf::RenderWindow* window);	//Zeichnet die Textur
	void render(sf::RenderWindow* window, sf::Vector2i& screenPosition, float scale = 1.0f, sf::Color tint = sf::Color::White); //Zeichnet die textur an einer bestimmten Stelle auf dem Bildschirm

	bool setPosition(sf::Vector2i& position);	//Setzt die Spielfeldposition und gibt zur�ck, ob es zu einer Kollision kommt.

	sf::Vector2i& getPosition();	//Gibt die Spielfeldposition zur�ck
	sf::Color& getTint();		//Gibt die F�rbung der Textur zur�ck

private:
	sf::Vector2i mPositionGrid;	//Position auf dem Spielfeld
	sf::Vector2i mPositionScreen;	//Position auf dem Bildschirm
	sf::Color mTint;		//F�rbung der Textur
	
	sf::Sprite m_sprite;
};