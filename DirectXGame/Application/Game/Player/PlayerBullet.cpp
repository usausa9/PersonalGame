#include "PlayerBullet.h"

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos, const Vector3& vel)
{
	// モデル紐づけ(仮引数で貰ってくる)
	bulletObj.objModel = model;

	// 行列更新,位置更新
	bulletObj.position = pos;
	bulletObj.InitializeObject3D();

	// 引数で移動量を受け取る
	velocity = vel;
}

void PlayerBullet::Update()
{
	// 移動量から座標を移動
	bulletObj.position += velocity;

	// 行列更新
	bulletObj.UpdateObject3D();
}

void PlayerBullet::Draw()
{
	// オブジェクト描画
	bulletObj.DrawObject3D();
}
