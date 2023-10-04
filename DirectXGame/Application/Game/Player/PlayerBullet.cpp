#include "PlayerBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

bool PlayerBullet::isDrawSp_ = false;

void PlayerBullet::Initialize(bool bulletState, OBJModel* model, const Vector3& pos, const Vector3& vel)
{
	// ���f���R�Â�(�������Ŗ���Ă���)
	objModel_ = model;

	// �s��X�V
	position_ = pos;
	if (bulletState == true)
	{
		scale_ = SMALL_SCALE_;
	}
	else
	{
		scale_ = EXPAND_SCALE_;
	}
	InitializeObject3D();

	// �����ňړ��ʂ��󂯎��
	velocity_ = vel;

	// �R���C�_�[�̒ǉ� (�����X�e�[�g�ɂ�肯��e�̑傫����ύX)
	float radius = 0.f;
	if (bulletState == true)
	{
		radius = EXPAND_COLLIDER_SIZE_;
	}
	else
	{
		radius = SMALL_COLLIDER_SIZE_;
	}
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);
}

void PlayerBullet::Update()
{
	isDrawSp_ = false;
	
	// ���Ԍo�߂ŏ���
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	// �ړ��ʂ�����W���ړ�
	position_ += velocity_;

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
	isDrawSp_ = true;
}