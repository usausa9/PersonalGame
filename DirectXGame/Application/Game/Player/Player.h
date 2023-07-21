#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"
#include "Sprite.h"

class Player
{
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// (現状は)レティクルのドロー
	void DrawUI();

	// 親子付けのセッター
	void SetParent(Object3D* object) { parent = object; }

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

private: // メンバ関数
	// 入力受け付け + 移動
	void Move();

	// 自機弾発射
	void Shot();

	// レティクルのアップデート
	void reticleUpdate();

private: // 自機のメンバ変数
	// 自機モデル
	OBJModel playerModel;
	OBJModel bulletModel;
	Object3D playerObj;

	// レティクルモデル, スプライト
	OBJModel reticleModel;
	Object3D reticleObj;
	TextureIndex reticleTex;
	unique_ptr<Sprite> reticleSp = nullptr;

	// 自機の移動用
	Vector3 move = { 0,0,0 };
	const float Velocity = 0.65f;
	const float kYMove = 0.7f;

	// 範囲制限用
	const Vector2 kMoveLimit = { 25.f, 13.f };

private: // 自機以外のメンバ変数
	// レティクルの座標データ
	Vector2 reticlePos = { 640, 360 };

	// 自機弾のリスト
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// 自機弾のモデル
	OBJModel bulletModel;
	
	// 自機弾のスピード
	const float kBulletSpeed = 0.7f;
};
