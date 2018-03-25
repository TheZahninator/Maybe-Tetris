#pragma once

using namespace DirectX;

class Texture{
public:
	Texture(){ mWidth = 0; mHeight = 0; }
	Texture(ID3D11Device* device, const wchar_t* path);
	~Texture();

	void load(ID3D11Device* device, const wchar_t* path);

	void draw(SpriteBatch* spriteBatch, SimpleMath::Vector2 pos, RECT sourceRect, SimpleMath::Vector4 tint = Colors::White.v, float rotation = 0.0f, SimpleMath::Vector2 origin = SimpleMath::Vector2(0, 0), float scale = 1.0f);
	void draw(SpriteBatch* spriteBatch, SimpleMath::Vector2 pos, SimpleMath::Vector4 tint = Colors::White.v, float rotation = 0.0f, SimpleMath::Vector2 origin = SimpleMath::Vector2(0, 0), float scale = 1.0f);

	int getWidth(){ return mWidth; }
	int getHeight(){ return mHeight; }

	SimpleMath::Vector2 center(){ return SimpleMath::Vector2((float)mWidth / 2, (float)mHeight / 2); }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	int mWidth, mHeight;
};