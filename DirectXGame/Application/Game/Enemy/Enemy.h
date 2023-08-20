#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "TimeData.h"
#include "SplineCurve.h"

#include <list>

// 敵の種類
enum class EnemyKinds : uint8_t
{
	NORMAL = 0x0001,// 通常の敵
	POWER = 0x0002,// 自機が強化される
};

class Enemy : public Object3D
{
public:	// メンバ関数
	// 初期化処理
	void Initialize(std::vector<Vector3>& points, uint8_t enemyKind);

	// 更新処理
	void Update(const Matrix4& cameraMatrix);

	// 描画処理
	void Draw();

	// 発生処理
	void Spawn();

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	// 敵の生存状況を取得
	bool IsAlive()const { return isAlive_; }

	// 当たり判定状況を取得
	static bool IsOnCol() { return sIsPowerUp_; }

private: // 敵機のメンバ変数
	// 敵機モデル, 敵機オブジェクト
	OBJModel enemyModel_;

	// 敵の軌道
	SplineCurve trajectory_ = {};

	uint8_t enemyKind_ = 0;

	// 生存フラグ
	bool isAlive_ = true;
	// 強化可能フラグ
	static bool sIsPowerUp_;
};