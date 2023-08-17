#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "TimeData.h"
#include "SplineCurve.h"

#include <list>

class Enemy : public Object3D
{
public:	// メンバ関数
	// 初期化処理
	void Initialize(std::vector<Vector3>& points);

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
	bool IsAlive()const { return isAlive; }

	// 当たり判定状況を取得
	static bool IsOnCol() { return isOnCol; }

private: // 敵機のメンバ変数
	// 敵機モデル, 敵機オブジェクト
	OBJModel enemyModel;

	// 敵の軌道
	SplineCurve trajectory = {};

	// 生存フラグ
	bool isAlive = true;
	// 当たり判定フラグ
	static bool isOnCol;
};