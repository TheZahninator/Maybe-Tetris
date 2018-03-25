#pragma once

#include "TextureIDs.h"
#include "BlockColors.h"

class TextureManager{
public:
	void static init();
	static sf::Texture& getTexture(int id);

private:
	static std::map<int, sf::Texture> mTextures;

};