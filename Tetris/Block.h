#pragma once

#include "TextureManager.h"
#include "Field.h"

#define BLOCK_SIZE 16	//Blockgr��e in Pixeln

class Block{
public:
	Block(DirectX::SimpleMath::Vector2& position, DirectX::SimpleMath::Vector4& tint = BLOCK_COLOR_SINGLE);

	void render(DirectX::SpriteBatch* spriteBatch);	//Zeichnet die Textur
	void render(DirectX::SpriteBatch* spriteBatch, DirectX::SimpleMath::Vector2& screenPosition, float scale = 1.0f); //Zeichnet die textur an einer bestimmten Stelle auf dem Bildschirm

	bool setPosition(DirectX::SimpleMath::Vector2& position);	//Setzt die Spielfeldposition und gibt zur�ck, ob es zu einer Kollision kommt.

	DirectX::SimpleMath::Vector2& getPosition();	//Gibt die Spielfeldposition zur�ck
	DirectX::SimpleMath::Vector4& getTint();		//Gibt die F�rbung der Textur zur�ck

private:
	DirectX::SimpleMath::Vector2 mPositionGrid;	//Position auf dem Spielfeld
	DirectX::SimpleMath::Vector2 mPositionScreen;	//Position auf dem Bildschirm
	DirectX::SimpleMath::Vector4 mTint;		//F�rbung der Textur
	
};