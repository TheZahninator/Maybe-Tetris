#pragma once

#include "Texture.h"
#include "TextureIDs.h"
#include "BlockColors.h"

class TextureManager{
public:
	void static init(ID3D11Device* device);
	static Texture* getTexture(int id);

private:
	static std::map<int, std::unique_ptr<Texture>> mTextures;

};