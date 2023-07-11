#pragma once
#include "Camera.h"
#include "Object3D.h"

class RailCamera
{
public:	// �����o�֐�
	// �f�X�g���N�^
	~RailCamera();

	// ����������
	void Initialize(const Vector3& pos, const Vector3& rot);

	// �X�V����
	void Update();

private: // �����o�ϐ�
	// ���[���h�ϊ��f�[�^
	Object3D* worldTransform = nullptr;

	// �J����
	Camera* camera = nullptr;
};

