#pragma once
#include "Common.h"
#include "Vector3.h"
#include "OBJModel.h"

// �萔�o�b�t�@�p�f�[�^�\���� (3D�ϊ��s��)
struct Object3DConstBufferDataTransform
{
	Matrix4 mat; // 3D�ϊ��s��
};

class Object3D
{
public:
	// �萔�o�b�t�@ (�s��p)
	ID3D12Resource* constBuffTransform = nullptr;

	// �萔�o�b�t�@�}�b�v (�s��p)
	Object3DConstBufferDataTransform* constMapTransform = nullptr;

	// �A�t�B���ϊ����
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	// ���[���h�ϊ��s��
	Matrix4 matWorld = Matrix4::Identity();

	// �e�I�u�W�F�N�g�ւ̃|�C���^
	Object3D* parent = nullptr;

	OBJModel* objModel = nullptr;

public:
	// 3D�I�u�W�F�N�g������
	void InitializeObject3D();

	// 3D�I�u�W�F�N�g�X�V����
	void UpdateObject3D();

	// 3D�I�u�W�F�N�g�`�揈��
	void DrawObject3D();
};

