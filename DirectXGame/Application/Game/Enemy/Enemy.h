#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "TimeData.h"

#include <list>

class Enemy : public Object3D
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

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

private: // メンバ関数
	// 入力受け付け + 移動
	void Move();

private: // 自機のメンバ変数
	//TimeData tData = {};

	// 自機モデル, 自機オブジェクト
	OBJModel enemyModel;

	// 自機の移動用
	Vector3 move = { 0,0,0 };

	// 自機の移動スピード 
	const float velocity = 0.65f;

	// 自機のXに対してのYスピード
	const float kYMove = 0.7f;
};
