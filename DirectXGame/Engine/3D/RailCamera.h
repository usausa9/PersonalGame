#pragma once
#include "Camera.h"
#include "Object3D.h"
#include "Common.h"

class RailCamera
{
public:	// �����o�֐�
	// �f�X�g���N�^
	~RailCamera();

	// ����������
	void Initialize(const Vector3& pos, const Vector3& rot);

	// �X�V����
	void Update();

	// �I�u�W�F�N�g�̃Q�b�^�[
	Object3D* GetObject3d()const { return worldTransform; }

	// �J�����̃Q�b�^�[
	Camera* GetCamera()const { return camera; }

private: // �����o�ϐ�
	// ���[���h�ϊ��f�[�^
	Object3D* worldTransform = nullptr;

	// �J����
	Camera* camera = nullptr;
};

