#include "RailCamera.h"
#include "Input.h"

using namespace Input;

RailCamera::~RailCamera()
{
	delete worldTransform_;
	delete camera_;
}

void RailCamera::Initialize(const Vector3& pos, const Vector3& rot)
{
	worldTransform_ = new Object3D();
	camera_ = new Camera();

	// ���[���h�s��̏����ݒ�
	worldTransform_->InitializeObject3D();
	worldTransform_->position_ = pos;
	worldTransform_->rotation_ = rot;

	// �r���[�s��̏�����
	camera_->Initialize();

	// �X�v���C���Ȑ��̐���_
	Vector3 start{ 0,0,0 };
	Vector3 p1{ 100,250,150 };
	Vector3 p2{ 0,0,200 };
	Vector3 end{ 100,250,350 };

	std::vector<Vector3> points{ start,p1,p2,end };

	spline_.SetPositions(points);
}

void RailCamera::Update()
{
	// �X�v���C���Ȑ��ɂ���ē�����
	if (Key::Trigger(DIK_O))
	{
		spline_.MoveStart(2800.0f, true);
	}

	// �Ȑ��̃A�b�v�f�[�g
	spline_.Update();

	// ���[���h�s��̍��W�̐��l�����Z
	// �X�v���C���Ȑ��ɉ����Ĉړ�
	worldTransform_->position_ = spline_.GetNowPosition();

	// ���[���h�s��̉�]�̐��l�����Z
	worldTransform_->rotation_ += { 0, 0, 0 };

	// ���[���h�s��̍s��Čv�Z
	worldTransform_->UpdateObject3D();

	// ���[���J�����ɂ͏�Ƀ��[���h���W����
	camera_->position_ = {
		worldTransform_->matWorld_.m[3][0],
		worldTransform_->matWorld_.m[3][1],
		worldTransform_->matWorld_.m[3][2] };

	// ���[���h�O���x�N�g��
	Vector3 forward = { 0, 0, 1 };

	// ���[���J�����̉�]�𔽉f
	forward = Matrix4::Transform(forward, worldTransform_->matWorld_);

	// ���_+�O���x�N�g���Œ����_
	camera_->target_ = camera_->position_ + forward;

	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };

	// �J�����̏������forward�Ɠ��l�ɕϊ�
	camera_->up_ = Matrix4::Transform(up, worldTransform_->matWorld_);

	// �J�����X�V
	camera_->Update();
}