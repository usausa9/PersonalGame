#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"

#include <list>

class Enemy
{
public:	// メンバ関数
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 親子付けのセッター
	// void SetParent(Object3D* object) { enemyObj.parent = object; }

private: // メンバ関数
	// 入力受け付け + 移動
	void Move();

private: // 自機のメンバ変数
	// 自機モデル, 自機オブジェクト
	OBJModel enemyModel;
	Object3D enemyObj;

	// 自機の移動用
	Vector3 move = { 0,0,0 };

	// 自機の移動スピード 
	const float velocity = 0.65f;

	// 自機のXに対してのYスピード
	const float kYMove = 0.7f;
};
