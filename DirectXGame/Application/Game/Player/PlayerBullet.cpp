#include "PlayerBullet.h"

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos)
{
	// 行列更新,位置更新
	bulletObj.position = pos;
	bulletObj.InitializeObject3D();

	// モデル紐づけ(仮引数で貰ってくる)
	bulletObj.objModel = model;
}

void PlayerBullet::Update()
{
	bulletObj.UpdateObject3D();
}

void PlayerBullet::Draw()
{
	bulletObj.DrawObject3D();
}
