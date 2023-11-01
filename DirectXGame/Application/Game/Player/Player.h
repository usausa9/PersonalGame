#pragma once
/**
 * @file	Player.h
 * @brief	プレイヤーのデータ保持や初期化/更新/描画を行うもの (Managerに近しい)
**/

#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "Camera.h"
#include "TimeData.h"
#include "PlayerState.h"

#include <list>

class Player : public Object3D
{
private:
	const float DISTANCE_RETICLE_ = 40.0f;

public:	// メンバ関数
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update(bool isMove, bool isReticleFollow, Vector2 position);

	// ゲームオーバー
	void GameOver();

	// 描画処理
	void Draw();

	// (現状は)レティクルのドロー
	void DrawUI(float statePosY);

	// 親子付けのセッター
	void SetParent(Object3D* object) { parent_ = object; }

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

private: // メンバ関数
	// レティクルのアップデート
	void reticleUpdate(bool isFollow, Vector2 position);

	// 入力受け付け + 移動
	void Move();

	// 自機弾発射
	void Shot();

private: // 自機のメンバ変数
	// 自機モデル, 自機オブジェクト
	OBJModel playerModel_;

	OBJModel reticleModel_;
	Object3D reticleObj_;
	TextureIndex reticleTex_;
	unique_ptr<Sprite> reticleSprite_ = nullptr;

	PlayerState state_;

	float deadReticlePosY_ = 0.f;
	TimeData deadTimer_;
	const float RETICLE_DEAD_MOVE_TIMER_ = 90.f;
	float reticleAnimeMove_;
	const float RETICLE_END_POS_Y_ = -80.f;

	// 自機の移動用
	Vector3 move_ = { 0,0,0 };
	Vector3 rev_ = { 0,0,0 };

	// 自機の初期位置
	const Vector3 INIT_PLAYER_POSITION_ = { 0, 0, -15 };

	// 自機の移動スピード 
	float velocity_ = 0.5f;
	const float FORMERLY_SPEED_ = 0.5f;
	const float SPEED_UP_RATE_ = 0.06f;

	// 自機から見た弾の発射地点
	const Vector3 DELAY_POS_ = { 0, 0.2f, 7.1f };
	Vector3 delayPos = { 0, 0, 0 };

	// 自機の当たり判定用
	const float RADIUS_ = 0.6f;

	// 自機のXに対してのYスピード
	const float kY_MOVE_ = 0.7f;
	// 自機の移動範囲制限用
	const Vector2 kMOVE_LIMIT_ = { 32.f, 17.f };

	// 自機の回転量 
	const float ROTATE_REV_ = 0.03f;
	// 自機Y回転量
	const float kY_ROTATE_ = 0.85f;
	// 自機の回転範囲制限用
	const Vector3 kREV_LIMIT_ = { 0.3f, 0.f, 0.3f };

	// レティクル関連
	float reticleSpd_ = 6.0f;
	const float kY_MOVE_RETICLE_ = 0.7f;
	const float kY_MOVE_LIMIT_ = 0.6f;
	const float RETICLE_MOVE_LIMIT_ = 340.f;
	const Vector3 INIT_RETICLE_POSITION_ = { 0, 0, 50 };
	const Vector2 RETICLE_SMALL_ = { 0.9f, 0.9f };
	const Vector2 RETICLE_BIG_ = { 1.1f, 1.1f };

	// ビューポート用
	const float kHALF_ = 0.500000f;

	float kReticleSpd_ = 6.0f / 0.65f;

	// ゲームオーバー用
	bool isGameOver_ = false;

private: // 自機弾のメンバ変数
	// レティクルの座標データ
	Vector2 reticlePos_ = { 640, 360 };

	// 自機弾のリスト
	std::list<unique_ptr<PlayerBullet>> bullets_ = {};

	// 自機弾のモデル
	OBJModel bulletModel_;

	// 自機弾のスピード
	const float kBULLET_SPEED_ = 2.2f;

	// 弾発射の間隔用
	TimeData shotTimeData_ = {};
	const float SHOT_DELAY_ = 1.f;
	const float SHOT_INTERVAL_ = 12.f;
};
