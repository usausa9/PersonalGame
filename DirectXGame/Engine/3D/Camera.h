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
	ID3D12Resource* constBuffCamera = nullptr;

	// �萔�o�b�t�@�}�b�v (�s��p)
	CameraConstBufferData* constMapCamera = nullptr;

	// �A�t�B���ϊ����
	// Float3 rotation = { 0,0,0 };
	Vector3 target = { 0,0,0 };
	Vector3 position = { 0,0,-20 };

	// 3D�p�̎ˉe�ϊ�
	Matrix4 matProjection = Matrix4::Identity();

	// �r���[�ϊ��s��
	Matrix4 matView = matView.Identity();
	Vector3 up = { 0,1,0 };
	
	// �r���{�[�h�s��
	Matrix4 matBillboard = Matrix4::Identity();

public:
	// �J����������
	void Initialize();

	// �J�����X�V����
	void Update();

	void Set();

	// �r���[�v���W�F�N�V����
	Matrix4 GetViewProjection()const { return matView * matProjection; }
	
	// �J�������W�擾
	Vector3 GetEye()const { return position; }
};