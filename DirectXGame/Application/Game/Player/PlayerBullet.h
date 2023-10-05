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
	void Initialize(bool bulletState, OBJModel* model, const Vector3& pos, const Vector3& velocity);

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
	Vector3 velocity_ = {};

	// 寿命<frame>
	static const int32_t s_kLIFE_TIME_ = 60 * 5;	// 5秒

	// 消滅タイマー
	int32_t deathTimer_ = s_kLIFE_TIME_;

	// 消滅フラグ
	bool isDead_ = false;

	// デバッグ用
	static bool isDrawSp_;

	// 自機強化に応じたサイズパラメータ
	const float SMALL_SIZE_ = 1.5f;
	const float EXPAND_SIZE_ = 2.2f;
	const float SMALL_COLLIDER_SIZE_ = 2.5f;
	const float EXPAND_COLLIDER_SIZE_ = 4.f;
	const Vector3 SMALL_SCALE_ = { SMALL_SIZE_, SMALL_SIZE_, SMALL_SIZE_ };
	const Vector3 EXPAND_SCALE_ = { EXPAND_SIZE_, EXPAND_SIZE_, EXPAND_SIZE_ };

public:
	// 外部参照用のGetter
	bool IsDead() const { return isDead_; }

	bool IsDrawSP() const { return isDrawSp_; }
};