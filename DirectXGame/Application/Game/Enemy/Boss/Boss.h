#pragma once

/**
 * @file	Boss.h
 * @brief	ボスのデータ保持や初期化/更新/描画を行うもの
**/

#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "SplineCurve.h"

class Boss : public Object3D
{
public:	// メンバ関数
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	void Spawn();

	// 敵の生存状況を取得
	bool IsAlive()const { return isAlive_; }

	// 親子付けのセッター
	void SetParent(Object3D* object) { parent_ = object; }

	//// 当たり判定状況を取得
	//static bool IsOnCol() { return sIsPowerUp_; }

private: // 敵機のメンバ変数
	// 敵機モデル, 敵機オブジェクト
	OBJModel bossModel_;

	// HP
	uint8_t hitPoints_ = 15;
	uint8_t deadHP_ = 0;

	// 生存フラグ
	bool isAlive_ = true;
	// 強化可能フラグ
	static bool sIsPowerUp_;

	const Vector3 INIT_BOSS_POSITION_ = {0, 5, 40};
	const Vector3 BOSS_DEAD_POSITION_ = { 10000, 0, -100 };

	// 敵の大きさ指定
	const float RADIUS_ = 5.2f;
	// 敵の軌道タイマー用
	const uint16_t ENEMY_MOVE_TIME_ = 600;
};