#include "Player.h"

using namespace Input;

// ������
void Player::Initialize()
{
	// ���@���f���ǂݍ���
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	
	// ���@�̍s�񏉊���
	playerObj.rotation = { -20 * (UsaMath::u_PI / 180), 0, 0 };
	playerObj.InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	playerObj.objModel = &playerModel;

	// ���@�e���f���ǂݍ���
	bulletModel = OBJModel::LoadFromOBJ("ICO");
}

// �X�V
void Player::Update()
{
	// ���Ńt���O���������e���폜
	bullets.remove_if([](unique_ptr<PlayerBullet>& bullet)
	{
		return bullet->IsDead();
	});

	// ���͂���̈ړ�����
	Move();

	// �s��X�V �K���Ăяo��
	playerObj.UpdateObject3D();

	// �e���� + �X�V
	Shot();
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update();
	}
}

// �`��
void Player::Draw()
{
	// �I�u�W�F�`��
	playerObj.DrawObject3D();

	// �e�`��
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Draw();
	}
}

// ���͎󂯕t�� + �ړ�
void Player::Move()
{
	// �ړ��ʂ̏�����
	move = { 0,0,0 };

	// WASD���͂ł̈ړ�
	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * velocity * kYMove,
		0 };

	// GamePad�ł̈ړ�
	move += {
		Pad::GetLStick().x * velocity,
		Pad::GetLStick().y * velocity * kYMove,
		0 };

	// �ړ��ʂ̉��Z
	playerObj.position += move;

	// �͈͐���
	playerObj.position.x = max(playerObj.position.x, -kMoveLimit.x);
	playerObj.position.y = max(playerObj.position.y, -kMoveLimit.y);
	playerObj.position.x = min(playerObj.position.x, +kMoveLimit.x);
	playerObj.position.y = min(playerObj.position.y, +kMoveLimit.y);
}

void Player::Shot()
{
	// �X�y�[�X�L�[�̃g���K�[���͂��󂯕t�����ꍇ
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		// ���@�e�̖��t���[���ړ�
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]
		velocity = velocity * playerObj.matWorld;
		
		// ���@�e�𐶐��A������
		bullets.push_back(std::move(make_unique<PlayerBullet>()));
		bullets.back()->Initialize(&bulletModel, playerObj.position + delayPos, velocity);
	}
}