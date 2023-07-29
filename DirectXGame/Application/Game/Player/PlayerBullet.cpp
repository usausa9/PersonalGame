#include "PlayerBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

bool PlayerBullet::isDrawSp = false;

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos, const Vector3& vel)
{
	// ���f���R�Â�(�������Ŗ���Ă���)
	objModel = model;

	// �s��X�V,�ʒu�X�V
	position = pos;
	InitializeObject3D();

	// �����ňړ��ʂ��󂯎��
	velocity = vel;

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void PlayerBullet::Update()
{
	isDrawSp = false;
	
	// ���Ԍo�߂ŏ���
	if (--deathTimer <= 0)
	{
		isDead = true;
	}

	// �ړ��ʂ�����W���ړ�
	position += velocity;

	// �s��X�V
	UpdateObject3D();
}

void PlayerBullet::Draw()
{
	// �I�u�W�F�N�g�`��
	DrawObject3D();
}

void PlayerBullet::OnCollision(const CollisionInfo& info)
{
	isDrawSp = true;
}