#include "PlayerBullet.h"

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos)
{
	bulletModel = OBJModel::LoadFromOBJ("ICO");
	
	bulletObj.InitializeObject3D();
	bulletObj.objModel = &bulletModel;
}

void PlayerBullet::Update()
{
	bulletObj.UpdateObject3D();
}

void PlayerBullet::Draw()
{
	bulletObj.DrawObject3D();
}
