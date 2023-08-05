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
	void Update(const Matrix4& cameraMatrix, const Vector3& playerWorldPosition);

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

private: // メンバ関数
	// 入力受け付け + 移動
	void Move();

private: // 自機のメンバ変数
	//TimeData tData = {};

	// 自機モデル, 自機オブジェクト
	OBJModel enemyModel;

	// 自機の移動用
	Vector3 move = { 0,0,0 };

	// 敵の軌道
	SplineCurve trajectory = {};

	// 自機の移動スピード 
	const float velocity = 0.65f;

	// 自機のXに対してのYスピード
	const float kYMove = 0.7f;

	// 生存フラグ
	bool isAlive = true;
};
