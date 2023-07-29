#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "Camera.h"

#include <list>

class Player : public Object3D
{
private:
	const float distanceReticle = 40.0f;

public:	// メンバ関数
	// 初期化処理
	void Initialize(Camera* camera);

	// 更新処理
	void Update();

	// 描画処理
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
	// レティクルのアップデート
	void reticleUpdate();

	// 入力受け付け + 移動
	void Move();

	// 自機弾発射
	void Shot();

private: // 自機のメンバ変数
	// 自機モデル, 自機オブジェクト
	OBJModel playerModel;

	OBJModel reticleModel;
	Object3D reticleObj;
	TextureIndex reticleTex;
	unique_ptr<Sprite> reticleSp = nullptr;

	TextureIndex aTex;
	unique_ptr<Sprite> aSp = nullptr;

	// 自機の移動用
	Vector3 move = { 0,0,0 };

	// 自機の移動スピード 
	const float velocity = 0.65f;

	// 自機のXに対してのYスピード
	const float kYMove = 0.7f;

	// 自機の移動範囲制限用
	const Vector2 kMoveLimit = { 25.f, 13.f };

private: // 自機弾のメンバ変数
	// レティクルの座標データ
	Vector2 reticlePos = { 640, 360 };

	// 自機弾のリスト
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// 自機弾のモデル
	OBJModel bulletModel;
	
	// 自機弾のスピード
	const float kBulletSpeed = 0.7f;
};
