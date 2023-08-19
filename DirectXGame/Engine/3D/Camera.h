#pragma once
#include "Common.h"

// �萔�o�b�t�@�p�f�[�^�\���� (3D�ϊ��s��)
struct CameraConstBufferData
{
	Matrix4 view; 
	Matrix4 projection;
	Matrix4 billboard;
	Vector3 position;
};

class Camera
{
public:
	~Camera();

public:
	// �萔�o�b�t�@ (�s��p)
	ID3D12Resource* constBuffCamera_ = nullptr;

	// �萔�o�b�t�@�}�b�v (�s��p)
	CameraConstBufferData* constMapCamera_ = nullptr;

	// �A�t�B���ϊ����
	Vector3 target_ = { 0,0,0 };
	Vector3 position_ = { 0,0,-20 };

	// 3D�p�̎ˉe�ϊ�
	Matrix4 matProjection_ = Matrix4::Identity();

	// �r���[�ϊ��s��
	Matrix4 matView_ = Matrix4::Identity();
	Vector3 up_ = { 0,1,0 };

	float nearZ_ = 0.1f;
	float farZ_ = 1500.0f;
	
	// �r���{�[�h�s��
	Matrix4 matBillboard_ = Matrix4::Identity();

	// ���g�p���Ă���J��������̃|�C���^�A�J�����ւ̃|�C���^
	static Camera* sCurrentCamera;

	static void SetCurrentCamera(Camera* current);

	static Camera* GetCurrentCamera();

public:
	// �J����������
	void Initialize();
	void Initialize(Vector3 position, Vector3 target, Vector3 up = {0, 1, 0});

	// �J�����X�V����
	void Update();

	// �J�����Z�b�g
	void Set();

	// �r���[�v���W�F�N�V����
	Matrix4 GetViewProjection()const { return matView_ * matProjection_; }
	
	// �J�������W�擾
	Vector3 GetEye()const { return position_; }
};