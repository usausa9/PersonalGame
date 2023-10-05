#pragma once
#include "Common.h"
#include "SpriteManager.h"

class Sprite
{
public:
	Sprite();
	Sprite(TextureIndex tex);
	~Sprite();

private:
	// 初期化
	void Init();

public:
	void Update();
	void Draw();

public: // メンバ変数
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView_{}; // 頂点バッファビューの作成

	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;	// 定数バッファ
	SpriteConstBufferDataMaterial* constMapMaterial_ = nullptr;// マップの割り当て

public:
	TextureIndex tIndex_ = 0;	// 描画するものを入れられるようにする

	Vector2 position_ = { 100,100 };
	Vector2 scale_ = { 1,1 };
	float rotation_ = 0;
};