#include "Player.h"
#include "SphereCollider.h"

using namespace Input;

// ������
void Player::Initialize()
{
	// ���@���f���ǂݍ���
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	
	// ���@�̍s�񏉊���
	rotation = { -7 * (UsaMath::u_PI / 180), 0, 0 };
	position = { 0, 0, 20 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel = &playerModel;

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }),radius));

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
	UpdateObject3D();

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
	DrawObject3D();

	// �e�`��
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Draw();
	}
}

void Player::OnCollision(const CollisionInfo& info)
{

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
	position += move;

	// �͈͐���
	position.x = max(position.x, -kMoveLimit.x);
	position.y = max(position.y, -kMoveLimit.y);
	position.x = min(position.x, +kMoveLimit.x);
	position.y = min(position.y, +kMoveLimit.y);
}

void Player::Shot()
{
	// �X�y�[�X�L�[ or Pad��A�{�^�� �̃g���K�[���͂��󂯕t�����ꍇ
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		// ���@�e�̖��t���[���ړ�
		Vector3 velocity(0, 0, kBulletSpeed);

		// ���@�e�̎��@����݂����[�J�����ˈʒu
		Vector3 delayPos = { 0, 0.2f, 7.1f };

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]
		velocity = velocity * matWorld;
		delayPos = delayPos * matWorld;
		
		// ���@�e�𐶐��A������
		bullets.push_back(std::move(make_unique<PlayerBullet>()));
		bullets.back()->Initialize(&bulletModel, GetWorldPosition() + delayPos, velocity);
	}
}