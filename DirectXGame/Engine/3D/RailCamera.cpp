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
	worldTransform->position += { 0,1,5 };

	// ���[���h�s��̉�]�̐��l�����Z
	worldTransform->rotation += { 0.05f, 0.05f, 0.05f };
	
	// ���[���h�s��̍s��Čv�Z
	worldTransform->UpdateObject3D();

	//camera->position = worldTransform->position;
}
