#include "Player.h"
#include "SphereCollider.h"
#include "WinAPI.h"
#include "CollisionAttribute.h"

using namespace Input;

// ������
void Player::Initialize(Camera* camera)
{
	// ���@���f���ǂݍ���
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	reticleModel = OBJModel::LoadFromOBJ("ICO");

	// ���e�B�N���X�v���C�g���蓖��
	reticleTex = TextureManager::Load(L"Resources/Sprites/reticle.png");
	reticleSp = make_unique<Sprite>(reticleTex);
	reticleSp->position = reticlePos;
	
	// ���@�̍s�񏉊���
	rotation = { 0, 0, 0 };
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
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

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

	// ���e�B�N���̍X�V
	reticleUpdate();

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

void Player::reticleUpdate()
{
	// �r���[�|�[�g�s��
	Matrix4 matViewPort = Matrix4::Identity(); 
	matViewPort.m[0][0] =   WinAPI::GetInstance()->width  / 2.0f;
	matViewPort.m[1][1] = -(WinAPI::GetInstance()->height / 2.0f);
	matViewPort.m[3][0] =   WinAPI::GetInstance()->width  / 2.0f;
	matViewPort.m[3][1] =   WinAPI::GetInstance()->height / 2.0f;

	// �J�����s��Ƃ̍���
	Matrix4 matViewProjectionViewPort = 
		Camera::GetCurrentCamera()->GetViewProjection() * matViewPort;

	// ��ʏ�̃��e�B�N�����W�𓮂���
	Vector2 reticleMoveVel = { 0, 0 };
	float reticleSpd = 4.0f;

	if (Key::Down(DIK_LEFT))
	{
		reticleMoveVel.x = -reticleSpd;
	}
	else if (Key::Down(DIK_RIGHT))
	{
		reticleMoveVel.x = reticleSpd;
	}

	if (Key::Down(DIK_UP))
	{
		reticleMoveVel.y = -reticleSpd;
	}
	else if (Key::Down(DIK_DOWN))
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

	// ���C�̕���
	Vector3 direction = posFar - posNear;
	direction.Normalize();

	// �J�������烌�e�B�N���̋���
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