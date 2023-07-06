#include "Player.h"

using namespace Input;

// ������
void Player::Initialize()
{
	playerModel = OBJModel::LoadFromOBJ("vicviper");

	playerObj.position = { 0, 0, 0 };
	playerObj.scale = { 1, 1, 1 };
	playerObj.rotation = { -20 * (UsaMath::u_PI / 180), 0, 0 };
	playerObj.InitializeObject3D();

	playerObj.objModel = &playerModel;
}

// �X�V
void Player::Update()
{
	// ���͂���̈ړ�����
	Move();

	// �s��X�V �K���Ăяo��
	playerObj.UpdateObject3D();
}

// �`��
void Player::Draw()
{
	// �I�u�W�F�`��
	playerObj.DrawObject3D();
}

// �ړ�
void Player::Move()
{
	move = { 0,0,0 };

	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * Velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * Velocity * 9 / 13,
		0 };
	
	playerObj.position += move;
}