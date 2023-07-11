#include "RailCamera.h"

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
}

void RailCamera::Update()
{
	// ���[���h�s��̍��W�̐��l�����Z

	// ���[���h�s��̉�]�̐��l�����Z
	
	// ���[���h�s��̍s��Čv�Z
	worldTransform->UpdateObject3D();
}
