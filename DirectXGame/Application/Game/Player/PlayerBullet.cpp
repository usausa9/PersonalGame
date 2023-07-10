#include "PlayerBullet.h"

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos)
{
	// �s��X�V,�ʒu�X�V
	bulletObj.position = pos;
	bulletObj.InitializeObject3D();

	// ���f���R�Â�(�������Ŗ���Ă���)
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
