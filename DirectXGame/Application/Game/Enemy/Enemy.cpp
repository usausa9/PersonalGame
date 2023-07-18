#include "Enemy.h"

// ������
void Enemy::Initialize()
{
	// ���@���f���ǂݍ���
	enemyModel = OBJModel::LoadFromOBJ("vicviper");

	// ���@�̍s�񏉊���
	enemyObj.rotation = { 0, 0, 0 };
	enemyObj.position = { 0, 0, 20 };
	enemyObj.InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	enemyObj.objModel = &enemyModel;
}

// �X�V
void Enemy::Update()
{
	// ���͂���̈ړ�����
	Move();

	// �s��X�V �K���Ăяo��
	enemyObj.UpdateObject3D();
}

// �`��
void Enemy::Draw()
{
	// �I�u�W�F�`��
	enemyObj.DrawObject3D();
}

// ���͎󂯕t�� + �ړ�
void Enemy::Move()
{
	// �ړ��ʂ̏�����
	move = { 0,0,0 };


	// �ړ��ʂ̉��Z
	enemyObj.position += move;

}