#include "PlayerBullet.h"

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos, const Vector3& vel)
{
	// ���f���R�Â�(�������Ŗ���Ă���)
	bulletObj.objModel = model;

	// �s��X�V,�ʒu�X�V
	bulletObj.position = pos;
	bulletObj.InitializeObject3D();

	// �����ňړ��ʂ��󂯎��
	velocity = vel;
}

void PlayerBullet::Update()
{
	// �ړ��ʂ�����W���ړ�
	bulletObj.position += velocity;

	// �s��X�V
	bulletObj.UpdateObject3D();
}

void PlayerBullet::Draw()
{
	// �I�u�W�F�N�g�`��
	bulletObj.DrawObject3D();
}
