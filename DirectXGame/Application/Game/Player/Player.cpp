#include "Player.h"
#include "SphereCollider.h"
#include "WinAPI.h"
#include "CollisionAttribute.h"

using namespace Input;

// ������
void Player::Initialize(Camera* camera)
{
	// ���@���f���ǂݍ���
	playerModel_ = OBJModel::LoadFromOBJ("vicviper");
	reticleModel_ = OBJModel::LoadFromOBJ("sphere");

	// ���e�B�N���X�v���C�g���蓖��
	reticleTex_ = TextureManager::Load(L"Resources/Sprites/reticle.png");
	reticleSp_ = make_unique<Sprite>(reticleTex_);
	reticleSp_->position_ = reticlePos_;

	// �����蔻��f�o�b�O�p�X�v���C�g
	aTex_ = TextureManager::Load(L"Resources/Sprites/texture.png");
	aSp_ = make_unique<Sprite>(aTex_);
	aSp_->position_ = { 100, 100 };
	
	// ���@�̍s�񏉊���
	rotation_ = { 0, 0, 0 };
	position_ = { 0, 0, 22 };
	InitializeObject3D();

	// ���@���f���Ǝ��@�I�u�W�F�N�g��R�Â�
	objModel_ = &playerModel_;
	reticleObj_.objModel_ = &reticleModel_;
	reticleObj_.position_ = { 0, 0, 50 };
	reticleObj_.InitializeObject3D();

	// �v���C���[��Ԃ̏�����
	state_.Initialize();

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }),radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	// ���@�e���f���ǂݍ���
	bulletModel_ = OBJModel::LoadFromOBJ("sphere");
}

// �X�V
void Player::Update()
{
	// �v���C���[��Ԃ̍X�V
	state_.Update();

	// ���Ńt���O���������e���폜
	bullets_.remove_if([](unique_ptr<PlayerBullet>& bullet)
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
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

// �`��
void Player::Draw()
{
	// �I�u�W�F�`��
	DrawObject3D();
	reticleObj_.DrawObject3D();

	// �e�`��
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
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
	matViewPort.m[0][0] =   WinAPI::GetInstance()->width_  / 2.0f;
	matViewPort.m[1][1] = -(WinAPI::GetInstance()->height_ / 2.0f);
	matViewPort.m[3][0] =   WinAPI::GetInstance()->width_  / 2.0f;
	matViewPort.m[3][1] =   WinAPI::GetInstance()->height_ / 2.0f;

	// �J�����s��Ƃ̍���
	Matrix4 matViewProjectionViewPort = 
		Camera::GetCurrentCamera()->GetViewProjection() * matViewPort;
	
	// ��ʏ�̃��e�B�N�����W�𓮂���
	Vector2 reticleMoveVel = { 0, 0 };

	if (state_.ExpandNum() == false)
	{
		reticleSp_->scale_ = { 0.9f, 0.9f };
	}
	else
	{
		reticleSp_->scale_ = { 1.1f, 1.1f };
	}

	// ���@�̑����ƃ��e�B�N���̃X�s�[�h����
	reticleSpd_ = kReticleSpd_ * velocity_;

	if (Key::Down(DIK_A) && Key::Down(DIK_D))
	{

	}
	else if (Key::Down(DIK_A))
	{
		reticleMoveVel.x = -reticleSpd_;
	}
	else if (Key::Down(DIK_D))
	{
		reticleMoveVel.x = reticleSpd_;
	}

	if (Key::Down(DIK_W) && Key::Down(DIK_S))
	{

	}
	else if (Key::Down(DIK_W))
	{
		reticleMoveVel.y = -reticleSpd_ * kY_MOVE_RETICLE_;
	}
	else if (Key::Down(DIK_S))
	{
		reticleMoveVel.y = reticleSpd_ * kY_MOVE_RETICLE_;
	}

	reticlePos_ += reticleMoveVel;

	// ���e�B�N�����W�̈ړ�����
	Vector2 reticlePosMin = { RETICLE_MOVE_LIMIT_, RETICLE_MOVE_LIMIT_ * 0.6f };
	Vector2 reticlePosMax = { WinAPI::GetInstance()->width_  - RETICLE_MOVE_LIMIT_,
							  WinAPI::GetInstance()->height_ - RETICLE_MOVE_LIMIT_ * 0.6f };
	
	reticlePos_.x = max(reticlePos_.x, reticlePosMin.x);
	reticlePos_.y = max(reticlePos_.y, reticlePosMin.y);
	reticlePos_.x = min(reticlePos_.x, reticlePosMax.x);
	reticlePos_.y = min(reticlePos_.y, reticlePosMax.y);

	// ���W���X�v���C�g�ɃZ�b�g
	reticleSp_->position_ = reticlePos_;

	// �����s��̐���
	Matrix4 matInverseVBV = matViewProjectionViewPort;
	matInverseVBV = Matrix4::Inverse(matInverseVBV);

	// �X�N���[�����W
	Vector3 posNear = { reticlePos_.x, reticlePos_.y, 0 };
	Vector3 posFar = { reticlePos_.x, reticlePos_.y, 1 };

	// �X�N���[�����W�n���烏�[���h���W�n��
	posNear = Matrix4::TransformDivW(posNear, matInverseVBV);
	posFar = Matrix4::TransformDivW(posFar, matInverseVBV);

	// ���C�̕���
	Vector3 direction = posFar - posNear;
	direction.Normalize();

	// �J�������烌�e�B�N���̋���
	reticleObj_.position_ = posNear + direction * DISTANCE_RETICLE_;

	reticleObj_.UpdateObject3D();
	reticleSp_->Update();
}

void Player::DrawUI()
{
	reticleSp_->Draw();
	
	// ���莞�`��
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		if (bullet->IsDrawSP())
		{
			//aSp_->Draw();
		};
	}

	// ���@��Ԃ̕`��
	state_.DrawUI();
}

// ���͎󂯕t�� + �ړ�
void Player::Move()
{
	// �ړ��ʂ̏�����
	move_ = { 0,0,0 };

	// �����ɉ����ăX�s�[�h�ω�
	velocity_ = FORMERLY_SPEED_ + (SPEED_UP_RATE_ * state_.SpeedUpNum());

	// WASD���͂ł̈ړ�
	move_ += {
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * velocity_,
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * velocity_ * kY_MOVE_,
		0 };

	/*rev = {
		(Key::Down(DIK_S) - Key::Down(DIK_W)) * rotateRev,
		0,
		((Key::Down(DIK_A) - Key::Down(DIK_D)) * rotateRev) * kYRotate };*/

	// GamePad�ł̈ړ�
	move_ += {
		Pad::GetLStick().x * velocity_,
		Pad::GetLStick().y * velocity_ * kY_MOVE_,
		0 };

	// �ړ��ʂ̉��Z
	position_ += move_;
	/*rotation += rev;*/

	// �͈͐���
	position_.x = max(position_.x, -kMOVE_LIMIT_.x);
	position_.y = max(position_.y, -kMOVE_LIMIT_.y);
	position_.x = min(position_.x, +kMOVE_LIMIT_.x);
	position_.y = min(position_.y, +kMOVE_LIMIT_.y);

	// �͈͐���
	rotation_.x = max(rotation_.x, -kREV_LIMIT_.x);
	rotation_.z = max(rotation_.z, -kREV_LIMIT_.z);
	rotation_.x = min(rotation_.x, +kREV_LIMIT_.x);
	rotation_.z = min(rotation_.z, +kREV_LIMIT_.z);
}

void Player::Shot()
{
	// �X�y�[�X�L�[ or Pad��A�{�^�� �̃g���K�[���͂��󂯕t�����ꍇ
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		shotTimeData_.Start(SHOT_INTERVAL_);
	}

	// �X�y�[�X�L�[ or Pad��A�{�^�� �̉������͂��󂯕t�����ꍇ
	if (Key::Down(DIK_SPACE) || Pad::Down(Button::A))
	{
		// �V���b�g�̃^�C�}�[���C���^�[�o���Ɠ������Ȃ����Ƃ��ɂ�����x�^�C�}�[�𓮂��� (�������Ă���̂�)
		if ((shotTimeData_.GetTime()) == SHOT_INTERVAL_)
		{
			shotTimeData_.Start(SHOT_INTERVAL_);
		}

		if (shotTimeData_.GetTime() == SHOT_DELAY_)
		{
			// ���@�e�̖��t���[���ړ�
			Vector3 velocity = { 0, 0, 0 };
			velocity = reticleObj_.GetWorldPosition() - Object3D::GetWorldPosition();
			velocity.Normalize();
			velocity *= kBULLET_SPEED_;

			// ���@�e�̎��@����݂����[�J�����ˈʒu
			Vector3 delayPos = { 0, 0.2f, 7.1f };

			// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]
			velocity = Matrix4::Transform(velocity, matWorld_);
			//velocity = velocity * matWorld;
			delayPos = delayPos * matWorld_;
	
			// ���@�e�𐶐��A������
			bullets_.push_back(std::move(make_unique<PlayerBullet>()));


			bullets_.back()->Initialize(state_.ExpandNum(), &bulletModel_, GetWorldPosition() + delayPos, velocity);
		}
	}
	// �^�C�}�[�̃A�b�v�f�[�g
	shotTimeData_.Update();
}