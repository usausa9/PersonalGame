#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"

class Player
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	void Move();

private:
	// モデル読み込み
	OBJModel playerModel;
	Object3D playerObj;

	Vector3 position = { 0,0,0 };
	Vector3 move = { 0,0,0 };
	const float Velocity = 0.65f;
	const float kYMove = 0.7f;

	// 範囲制限用
	const Vector3 kMoveLimit = { 25.f, 13.f, 1.0f };
};
