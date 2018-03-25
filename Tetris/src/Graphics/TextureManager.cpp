#include <pch.h>
#include "TextureManager.h"

std::map<int, sf::Texture> TextureManager::mTextures;

void TextureManager::init(){
	//Load Textures
	mTextures[TEX_BLOCK_WHITE] = sf::Texture();
	mTextures[TEX_BLOCK_WHITE].loadFromFile("res/blockWhite.png");

	mTextures[TEX_BACKGROUND_FIELD_BORDER] = sf::Texture();
	mTextures[TEX_BACKGROUND_FIELD_BORDER].loadFromFile("res/fieldBackground.png");
	
	mTextures[TEX_BACKGROUND_FIELD_TILE] = sf::Texture();
	mTextures[TEX_BACKGROUND_FIELD_TILE].loadFromFile("res/fieldBackgroundTile.png");
	
	mTextures[TEX_BACKGROUND_1] = sf::Texture();
	mTextures[TEX_BACKGROUND_1].loadFromFile("res/bg.png");
	
	mTextures[TEX_BACKGROUND_OVERLAY] = sf::Texture();
	mTextures[TEX_BACKGROUND_OVERLAY].loadFromFile("res/tetris_overlay.png");
	
	mTextures[TEX_KEY_OVERLAY] = sf::Texture();
	mTextures[TEX_KEY_OVERLAY].loadFromFile("res/key_overlay.png");
}

sf:: Texture& TextureManager::getTexture(int id){
	return mTextures[id];
}
