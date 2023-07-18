#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"

#include <list>

class Player
{
public:	// メンバ関数
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 親子付けのセッター
	void SetParent(Object3D* object) { playerObj.parent = object; }

private: // メンバ関数
	// 入力受け付け + 移動
	void Move();

	// 自機弾発射
	void Shot();

private: // 自機のメンバ変数
	// 自機モデル, 自機オブジェクト
	OBJModel playerModel;
	Object3D playerObj;

	// 自機の移動用
	Vector3 move = { 0,0,0 };

	// 自機の移動スピード 
	const float velocity = 0.65f;

	// 自機のXに対してのYスピード
	const float kYMove = 0.7f;

	// 自機の移動範囲制限用
	const Vector2 kMoveLimit = { 25.f, 13.f };

private: // 自機弾のメンバ変数
	// 自機弾のリスト
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// 自機弾のモデル
	OBJModel bulletModel;
	
	// 自機弾のスピード
	const float kBulletSpeed = 0.7f;
};
