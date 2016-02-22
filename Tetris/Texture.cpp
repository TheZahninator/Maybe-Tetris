#include "pch.h"
#include "Texture.h"

Texture::Texture(ID3D11Device* device, const wchar_t* path){
	load(device, path);
}

Texture::~Texture(){
	m_texture.Reset();
}

void Texture::load(ID3D11Device*device, const wchar_t* path){
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, path, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf()));

	//Get the resolution of the texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(resource.As(&tex));

	CD3D11_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);

	if (!mWidth)
		mWidth = 0;
	if (!mHeight)
		mHeight = 0;

	mWidth = texDesc.Width;
	mHeight = texDesc.Height;

}

void Texture::draw(SpriteBatch* spriteBatch, SimpleMath::Vector2 pos, RECT sourceRect, SimpleMath::Vector4 tint, float rotation, SimpleMath::Vector2 origin, float scale){
	spriteBatch->Draw(m_texture.Get(), pos, &sourceRect, tint, rotation, origin, scale);
}

void Texture::draw(SpriteBatch* spriteBatch, SimpleMath::Vector2 pos, SimpleMath::Vector4 tint, float rotation, SimpleMath::Vector2 origin, float scale){
	RECT sourceRect;
	sourceRect.left = 0;
	sourceRect.top = 0;
	sourceRect.right = mWidth;
	sourceRect.bottom = mHeight;

	spriteBatch->Draw(m_texture.Get(), pos, &sourceRect, tint, rotation, origin, scale);
}

