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

	// 強化状況を取得
	uint8_t SpeedUpNum() const { return powerUpStatus_[0]; }
	uint8_t ExpandNum()  const { return powerUpStatus_[1]; }
	uint8_t LaserNum()   const { return powerUpStatus_[2]; }
	uint8_t OptionNum()  const { return powerUpStatus_[3]; }
	uint8_t BarrierNum() const { return powerUpStatus_[4]; }

private:
	// パワーアップアイテム関連
	array<uint8_t, 5> powerUpSelect_ = { 0, 0, 0, 0, 0 };	// 現在の選択等描画用
	array<uint8_t, 5> powerUpStatus_ = { 0, 0, 0, 0, 0 };	// プレイヤーの強化状態
	uint8_t nowSelect_ = uint8_t(State::_NONE);
	
	const uint8_t maxSpeedUpNum_ = 5;
	const uint8_t maxExpandNum_ = 1;
	const uint8_t maxLaserNum_ = 1;
	const uint8_t maxOptionNum_ = 2;
	const uint8_t maxBarrierNum_ = 1;

private:// 描画用
	// テクスチャ関連
	static const uint8_t powerUpNum_ = 5;
	static const uint8_t texNum_ = 12;
	static const uint8_t spriteNum_ = 20;
	array<TextureIndex, texNum_> stateSelectTex_ = {};
	array<unique_ptr<Sprite>, spriteNum_> stateSprite_ = {
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, };

	const float scaleTexUI_ = 1.5f;
	const Vector2 spriteBase_ = { 337, 667 };
	static const uint8_t spriteSize_ = 150;
};