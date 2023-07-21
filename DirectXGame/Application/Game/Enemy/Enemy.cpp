#include "Enemy.h"
#include "SphereCollider.h"

// ������
void Enemy::Initialize()
{
	// ���@���f���ǂݍ���
	enemyModel = OBJModel::LoadFromOBJ("Cube");

	// ���@�̍s�񏉊���
	rotation = { 0, 0, 0 };
	position = { 0, 0, 50 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel = &enemyModel;

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
}

// �X�V
void Enemy::Update()
{
	// �ړ�����
	//Move();

	// �s��X�V �K���Ăяo��
	UpdateObject3D();
}

// �`��
void Enemy::Draw()
{
	// �I�u�W�F�`��
	DrawObject3D();
}

void Enemy::OnCollision(const CollisionInfo& info)
{

}

// ���͎󂯕t�� + �ړ�
void Enemy::Move()
{
	// �ړ��ʂ̏�����
	move = { 0,0,0 };

	// �ړ��ʂ̉��Z
	position += move;
}