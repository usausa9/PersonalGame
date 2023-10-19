#pragma once

/**
 * @file	Sprite.h
 * @brief	スプライトのデータ保持/初期化/更新/描画等を行うもの
**/

#include "Common.h"
#include "SpriteManager.h"

class Sprite
{
public:
	// スプライトのコンストラクタ
	Sprite();

	// テクスチャを指定したInitの呼び出し
	Sprite(TextureIndex tex);

	// スプライトのデストラクタ
	~Sprite();

private:
	// 初期化
	void Init();

public:
	// 更新処理
	void Update();

	// 描画処理
	void Draw();

public: // メンバ変数
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView_{}; // 頂点バッファビューの作成

	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;	// 定数バッファ
	SpriteConstBufferDataMaterial* constMapMaterial_ = nullptr;// マップの割り当て

public:
	TextureIndex texIndex_ = 0;	// 描画するものを入れられるようにする

	Vector2 position_ = { 100,100 };
	Vector2 scale_ = { 1,1 };
	float rotation_ = 0;
};