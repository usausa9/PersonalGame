#pragma once
#include "Common.h"
#include "UsaMath.h"
#include "OBJModel.h"

#include "CollisionInfo.h"
#include"CollisionManager.h"

class BaseCollider;

// �萔�o�b�t�@�p�f�[�^�\���� (3D�ϊ��s��)
struct Object3DConstBufferDataTransform
{
	Matrix4 mat; // 3D�ϊ��s��
};

class Object3D
{
public: // �����o�ϐ�
	// �萔�o�b�t�@ (�s��p)
	ID3D12Resource* constBuffTransform = nullptr;

	// �萔�o�b�t�@�}�b�v (�s��p)
	Object3DConstBufferDataTransform* constMapTransform = nullptr;

	// �A�t�B���ϊ����
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	Vector3 GetWorldPosition() const { return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] }; }

	// ���[���h�ϊ��s��
	Matrix4 matWorld = Matrix4::Identity();

	// �e�I�u�W�F�N�g�ւ̃|�C���^
	Object3D* parent = nullptr;

	// ���f���̃|�C���^
	OBJModel* objModel = nullptr;

	// �N���X�� (�f�o�b�O�p)
	const char* name = nullptr;

	// �R���C�_�[
	BaseCollider* collider = nullptr;

public:
	// �R���X�g���N�^
	Object3D() = default;

	// �f�X�g���N�^
	virtual ~Object3D();

	// 3D�I�u�W�F�N�g������
	virtual void InitializeObject3D();

	// 3D�I�u�W�F�N�g�X�V����
	virtual void UpdateObject3D();

	// 3D�I�u�W�F�N�g�`�揈��
	virtual void DrawObject3D();

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const Matrix4& GetMatWorld() { return matWorld; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}
};