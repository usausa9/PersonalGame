#pragma once
#include "OBJModel.h"
#include "Object3D.h"
#include "Sprite.h"

class PlayerBullet : public Object3D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">自機弾 モデル</param>
	/// <param name="pos">発射位置</param>
	void Initialize(OBJModel* model, const Vector3& pos, const Vector3& velocity);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

private: 
	// 自機弾の毎フレーム移動量
	Vector3 velocity = {};

	// 寿命<frame>
	static const int32_t kLifeTime = 60 * 5;	// 5秒
	
	// 消滅タイマー
	int32_t deathTimer = kLifeTime;

	// 消滅フラグ
	bool isDead = false;

	// デバッグ用
	static bool isDrawSp;

public:
	// 外部参照用のGetter
	bool IsDead() const { return isDead; }
	
	bool IsDrawSP() const { return isDrawSp; }
};