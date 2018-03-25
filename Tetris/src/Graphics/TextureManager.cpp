#include <pch.h>
#include "TextureManager.h"

std::map<int, std::unique_ptr<Texture>> TextureManager::mTextures;

void TextureManager::init(ID3D11Device* device){
	//Load Textures
	mTextures[TEX_BLOCK_WHITE].reset(new Texture(device, L"res/blockWhite.png"));
	mTextures[TEX_BACKGROUND_FIELD_BORDER].reset(new Texture(device, L"res/fieldBackground.png"));
	mTextures[TEX_BACKGROUND_FIELD_TILE].reset(new Texture(device, L"res/fieldBackgroundTile.png"));
	mTextures[TEX_BACKGROUND_1].reset(new Texture(device, L"res/bg.png"));
	mTextures[TEX_BACKGROUND_OVERLAY].reset(new Texture(device, L"res/tetris_overlay.png"));
	mTextures[TEX_KEY_OVERLAY].reset(new Texture(device, L"res/key_overlay.png"));
}

Texture* TextureManager::getTexture(int id){
	return mTextures[id].get();
}
