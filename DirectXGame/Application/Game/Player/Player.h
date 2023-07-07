#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"

class Player
{
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 入力受け付け + 移動
	void Move();

	// 自機弾発射
	void Shot();

private:
	// 自機弾
	unique_ptr<PlayerBullet> bullet = nullptr;

	// モデル読み込み
	OBJModel playerModel;
	Object3D playerObj;

	Vector3 position = { 0,0,0 };
	Vector3 move = { 0,0,0 };
	const float Velocity = 0.65f;
	const float kYMove = 0.7f;

	// 範囲制限用
	const Vector2 kMoveLimit = { 25.f, 13.f };
};
