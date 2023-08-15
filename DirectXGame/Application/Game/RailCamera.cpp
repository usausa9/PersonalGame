#include "RailCamera.h"
#include "Input.h"

using namespace Input;

RailCamera::~RailCamera()
{
	delete worldTransform;
	delete camera;
}

void RailCamera::Initialize(const Vector3& pos, const Vector3& rot)
{
	worldTransform = new Object3D();
	camera = new Camera();

	// ���[���h�s��̏����ݒ�
	worldTransform->InitializeObject3D();
	worldTransform->position = pos;
	worldTransform->rotation = rot;

	// �r���[�s��̏�����
	camera->Initialize();


	// �X�v���C���Ȑ��̐���_
	Vector3 start{ 0,0,0 };
	Vector3 p1{ 0,2,250 };
	Vector3 p2{ 0,4,500 };
	Vector3 end{ 0,6,750 };

	std::vector<Vector3> points{ start,p1,p2,end };

	spline.SetPositions(points);
}

void RailCamera::Update()
{
	// �X�v���C���Ȑ��ɂ���ē�����
	if (Key::Trigger(DIK_O)) 
	{
		spline.MoveStart(6000.0f, true);
	}

	// �Ȑ��̃A�b�v�f�[�g
	spline.Update();

	// ���[���h�s��̍��W�̐��l�����Z
	// �X�v���C���Ȑ��ɉ����Ĉړ�
	worldTransform->position = spline.GetNowPosition();

	// ���[���h�s��̉�]�̐��l�����Z
	worldTransform->rotation += { 0, 0, 0 };
	
	// ���[���h�s��̍s��Čv�Z
	worldTransform->UpdateObject3D();

	// ���[���J�����ɂ͏�Ƀ��[���h���W����
	camera->position = {
		worldTransform->matWorld.m[3][0],
		worldTransform->matWorld.m[3][1],
		worldTransform->matWorld.m[3][2]};

	// ���[���h�O���x�N�g��
	Vector3 forward = {0, 0, 1};

	// ���[���J�����̉�]�𔽉f
	forward = Matrix4::Transform(forward, worldTransform->matWorld);
	
	// ���_+�O���x�N�g���Œ����_
	camera->target = camera->position + forward;
	
	// ������x�N�g��
	Vector3 up = {0, 1, 0};
	
	// �J�����̏������forward�Ɠ��l�ɕϊ�
	camera->up = Matrix4::Transform(up, worldTransform->matWorld);
	
	// �J�����X�V
	camera->Update();

}
