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

	// �����蔻��f�o�b�O�p�X�v���C�g
	aTex = TextureManager::Load(L"Resources/Sprites/texture.png");
	aSp = make_unique<Sprite>(aTex);
	aSp->position = {100,100};
	
	// ���@�̍s�񏉊���
	rotation = { 0, 0, 0 };
	position = { 0, 0, 22 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel = &playerModel;
	reticleObj.objModel = &reticleModel;
	reticleObj.position = { 0, 0, 50 };
	reticleObj.InitializeObject3D();

	// �v���C���[��Ԃ̏�����
	state.Initialize();

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
	// �v���C���[��Ԃ̍X�V
	state.Update();

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
	reticleObj.DrawObject3D();

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

	// ���@�̑����ƃ��e�B�N���̃X�s�[�h����
	reticleSpd = kReticleSpd * velocity;

	if (Key::Down(DIK_A) && Key::Down(DIK_D))
	{

	}
	else if (Key::Down(DIK_A))
	{
		reticleMoveVel.x = -reticleSpd;
	}
	else if (Key::Down(DIK_D))
	{
		reticleMoveVel.x = reticleSpd;
	}

	if (Key::Down(DIK_W) && Key::Down(DIK_S))
	{

	}
	else if (Key::Down(DIK_W))
	{
		reticleMoveVel.y = -reticleSpd * kYMoveReticle;
	}
	else if (Key::Down(DIK_S))
	{
		reticleMoveVel.y = reticleSpd * kYMoveReticle;
	}

	reticlePos += reticleMoveVel;

	// ���e�B�N�����W�̈ړ�����
	Vector2 reticlePosMin = { reticleRadius, reticleRadius * 0.6f };
	Vector2 reticlePosMax = { WinAPI::GetInstance()->width  - reticleRadius,
							  WinAPI::GetInstance()->height - reticleRadius * 0.6f };
	
	reticlePos.x = max(reticlePos.x, reticlePosMin.x);
	reticlePos.y = max(reticlePos.y, reticlePosMin.y);
	reticlePos.x = min(reticlePos.x, reticlePosMax.x);
	reticlePos.y = min(reticlePos.y, reticlePosMax.y);

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
	
	// ���莞�`��
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		if (bullet->IsDrawSP())
		{
			aSp->Draw();
		};
	}

	// ���@��Ԃ̕`��
	state.DrawUI();
}

// ���͎󂯕t�� + �ړ�
void Player::Move()
{
	// �ړ��ʂ̏�����
	move = { 0,0,0 };

	// �����ɉ����ăX�s�[�h�ω�
	velocity = formerlySpeed + speedUpRate * state.SpeedUpNum();

	// WASD���͂ł̈ړ�
	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * velocity * kYMove,
		0 };

	/*rev = {
		(Key::Down(DIK_S) - Key::Down(DIK_W)) * rotateRev,
		0,
		((Key::Down(DIK_A) - Key::Down(DIK_D)) * rotateRev) * kYRotate };*/

	// GamePad�ł̈ړ�
	move += {
		Pad::GetLStick().x * velocity,
		Pad::GetLStick().y * velocity * kYMove,
		0 };

	// �ړ��ʂ̉��Z
	position += move;
	/*rotation += rev;*/

	// �͈͐���
	position.x = max(position.x, -kMoveLimit.x);
	position.y = max(position.y, -kMoveLimit.y);
	position.x = min(position.x, +kMoveLimit.x);
	position.y = min(position.y, +kMoveLimit.y);

	// �͈͐���
	rotation.x = max(rotation.x, -kRevLimit.x);
	rotation.z = max(rotation.z, -kRevLimit.z);
	rotation.x = min(rotation.x, +kRevLimit.x);
	rotation.z = min(rotation.z, +kRevLimit.z);
}

void Player::Shot()
{
	// �X�y�[�X�L�[ or Pad��A�{�^�� �̃g���K�[���͂��󂯕t�����ꍇ
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		shotTimeData.Start(shotInterval);
	}

	// �X�y�[�X�L�[ or Pad��A�{�^�� �̉������͂��󂯕t�����ꍇ
	if (Key::Down(DIK_SPACE) || Pad::Down(Button::A))
	{
		// �V���b�g�̃^�C�}�[���C���^�[�o���Ɠ������Ȃ����Ƃ��ɂ�����x�^�C�}�[�𓮂��� (�������Ă���̂�)
		if ((shotTimeData.GetTime()) == shotInterval)
		{
			shotTimeData.Start(shotInterval);
		}

		if (shotTimeData.GetTime() == shotDelay)
		{
			// ���@�e�̖��t���[���ړ�
			Vector3 velocity = { 0, 0, 0 };
			velocity = reticleObj.GetWorldPosition() - Object3D::GetWorldPosition();
			velocity.Normalize();
			velocity *= kBulletSpeed;

			// ���@�e�̎��@����݂����[�J�����ˈʒu
			Vector3 delayPos = { 0, 0.2f, 7.1f };

			// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]
			velocity = Matrix4::Transform(velocity, matWorld);
			//velocity = velocity * matWorld;
			delayPos = delayPos * matWorld;
	
			// ���@�e�𐶐��A������
			bullets.push_back(std::move(make_unique<PlayerBullet>()));
			bullets.back()->Initialize(&bulletModel, GetWorldPosition() + delayPos, velocity);
		}
	}
	// Timer�̃A�b�v�f�[�g
	shotTimeData.Update();
}