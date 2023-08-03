#pragma once

#include "Common.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

class PlayerState
{
public:
	enum class SelectState : uint8_t
	{
		NONE,		// 選ばれていない
		SELECT,		// 選ばれている
		USED,		// 使用済みであり選ばれてない
		USEDSELECT,	// 使用済みであり選ばれている
	};

	enum class State : uint8_t
	{
		SPEEDUP,	// 加速
		EXPAND,		// レティクルが大きくなる
		LASER,		// レーザー化
		OPTION,		// プレハブが増える
		BARRIER,	// ５回までダメージを受けられるバリア
		_NONE,		// どれも選ばれていない
	};

public:
	// 画像読み込み + 紐づけ
	void SetTexture();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void DrawUI();

private:
	// パワーアップアイテム関連
	array<uint8_t, 5> powerUpSelect = { 0, 0, 0, 0, 0 };	// 現在の選択等描画用
	array<uint8_t, 5> powerUpStatus = { 0, 0, 0, 0, 0 };	// プレイヤーの強化状態
	uint8_t nowSelect = uint8_t(State::_NONE);

	bool isPowerUpAble = false;
	

private:	// 描画用
	// テクスチャ関連
	static const uint8_t powerUpNum = 5;
	static const uint8_t texNum = 12;
	static const uint8_t spriteNum = 20;
	array<TextureIndex, texNum> stateSelectTex = {};
	array<unique_ptr<Sprite>, spriteNum> stateSprite = {
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, };

	const float scaleTexUI = 1.5f;
	const Vector2 spriteBase = { 337, 667 };
	static const uint8_t spriteSize = 150;

};