#include "PlayerBullet.h"

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos)
{
	bulletObj.InitializeObject3D();
}

void PlayerBullet::Update()
{
	bulletObj.UpdateObject3D();
}

void PlayerBullet::Draw()
{
	bulletObj.DrawObject3D();
}
