#include "Player.h"

using namespace Input;

// ������
void Player::Initialize()
{
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	
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

// ���͎󂯕t�� + �ړ�
void Player::Move()
{
	move = { 0,0,0 };

	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * Velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * Velocity * kYMove,
		0 };
	
	playerObj.position += move;

	// �͈͐���
	playerObj.position.x = max(playerObj.position.x, -kMoveLimit.x);
	playerObj.position.y = max(playerObj.position.y, -kMoveLimit.y);
	playerObj.position.x = min(playerObj.position.x, +kMoveLimit.x);
	playerObj.position.y = min(playerObj.position.y, +kMoveLimit.y);
}

void Player::Shot()
{
	if (Key::Trigger(DIK_SPACE))
	{
		// ���@�e�𐶐��A������
		unique_ptr<PlayerBullet> newBullet = make_unique<PlayerBullet>();
		//newBullet.get()->Initialize(bullet.get().bulletModel)
	}
}