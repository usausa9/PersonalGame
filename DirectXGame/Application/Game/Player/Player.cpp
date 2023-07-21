#include "Player.h"
#include "SphereCollider.h"
#include "WinAPI.h"

using namespace Input;

// ������
void Player::Initialize()
{
	// ���@���f�����ǂݍ���
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	reticleModel = OBJModel::LoadFromOBJ("ICO");

	// ���e�B�N���X�v���C�g���蓖��
	reticleTex = TextureManager::Load(L"Resources/Sprites/reticle.png");
	reticleSp = make_unique<Sprite>(reticleTex);
	reticleSp->position = reticlePos;
	
	// ���@�̍s�񏉊���
	rotation = { -7 * (UsaMath::u_PI / 180), 0, 0 };
	position = { 0, 0, 20 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel = &playerModel;
	reticleObj.objModel = &reticleModel; 
	reticleObj.position = { 0, 0, 50 };
	reticleObj.InitializeObject3D();

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }),radius));

	playerObj.objModel = &playerModel;
}

// �X�V
void Player::Update()
{
	// ���͂���̈ړ�����
	Move();

	// �s��X�V �K���Ăяo��
	UpdateObject3D();

	// ���e�B�N���̍X�V
	reticleUpdate();

	// �e���� + �X�V
	Shot();
	if (bullet) 
	{
		bullet->Update();
	}

	// �s��X�V �K���Ăяo��
	playerObj.UpdateObject3D();
}

// �`��
void Player::Draw()
{
	// �I�u�W�F�`��
	playerObj.DrawObject3D();

	// �e�`��
	if (bullet)
	{
		bullet->Draw();
	}
}

void Player::OnCollision(const CollisionInfo& info)
{

}

void Player::reticleUpdate()
{
	// �r���[�|�[�g�s��
	Matrix4 matViewPort;
	matViewPort.Identity();
	matViewPort.m[0][0] =   WinAPI::Get()->width  / 2.0f;
	matViewPort.m[1][1] = -(WinAPI::Get()->height / 2.0f);
	matViewPort.m[3][0] =   WinAPI::Get()->width  / 2.0f;
	matViewPort.m[3][1] =   WinAPI::Get()->height / 2.0f;

	// �J�����s��Ƃ̍���
	Matrix4 matViewProjectionViewPort = GetViewProjection() * matViewPort;

	// ��ʏ�̃��e�B�N�����W�𓮂���
	Vector2 reticleMoveVel = { 0, 0 };
	float reticleSpd = 4.0f;

	if (Key::Trigger(DIK_LEFT)) 
	{
		reticleMoveVel.x = -reticleSpd;
	}
	else if (Key::Trigger(DIK_RIGHT)) 
	{
		reticleMoveVel.x = reticleSpd;
	}

	if (Key::Trigger(DIK_UP)) 
	{
		reticleMoveVel.y = -reticleSpd;
	}
	else if (Key::Trigger(DIK_DOWN))
	{
		reticleMoveVel.y = reticleSpd;
	}

	reticlePos += reticleMoveVel;

	// ���W���X�v���C�g�ɃZ�b�g
	reticleSp->position = reticlePos;

	// �����s��̐���
	Matrix4 matInverseVBV = matViewProjectionViewPort;
	matInverseVBV = Matrix4::Inverse(matInverseVBV);

	// �X�N���[�����W
	Vector3 posNear = { reticlePos.x, reticlePos.y, 0 };
	Vector3 posFar = { reticlePos.x, reticlePos.y, 1 };

	// �X�N���[�����W�n���烏�[���h���W�n��
	posNear = Matrix4::TransformDivW(posNear, matInverseVBV);
	posFar = Matrix4::TransformDivW(posFar, matInverseVBV);

	// �}�E�X���C�̕���
	Vector3 direction = posFar - posNear;
	direction.Normalize();

	// �J�������烌�e�B�N���̋���
	const float distanceReticle = 100.0f;
	reticleObj.position = posNear + direction * distanceReticle;

	reticleObj.UpdateObject3D();
	reticleSp->Update();
}

void Player::DrawUI()
{
	reticleSp->Draw();
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
		// ���@�e�̖��t���[���ړ�
		Vector3 velocity = { 0, 0, 0 };
		velocity = reticleObj.GetWorldPosition() - Object3D::GetWorldPosition();
		velocity.Normalize();
		velocity *= kBulletSpeed;

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