#pragma once
#include "Camera.h"
#include "Object3D.h"
#include "Common.h"

#include "SplineCurve.h"

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
	Object3D* GetObject3d()const { return worldTransform_; }

	// �J�����̃Q�b�^�[
	Camera* GetCamera()const { return camera_; }

private: // �����o�ϐ�
	// ���[���h�ϊ��f�[�^
	Object3D* worldTransform_ = nullptr;

	// �J����
	Camera* camera_ = nullptr;

	// �J�����p�X�v���C���Ȑ�
	SplineCurve spline_;
};

