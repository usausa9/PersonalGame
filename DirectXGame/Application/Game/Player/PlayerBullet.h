#pragma once
#include "OBJModel.h"
#include "Object3d.h"

class PlayerBullet
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

private: 
	Object3D bulletObj;

	// 自機弾の毎フレーム移動量
	Vector3 velocity = {};

};