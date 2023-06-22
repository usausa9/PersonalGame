#pragma once
#include "FBXModel.h"
#include "FbxLoader.h"
#include "Camera.h"
#include "Common.h"

#include <Windows.h>

class FBXObject3D
{
protected:	// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �萔
	// �{�[���̍ő吔
	static const int MAX_BONES = 32;

public:	// �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���� (���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		Matrix4 viewProjection;	// �r���[�v���W�F�N�V�����s��
		Matrix4 worldTransform;	// ���[���h�s��
		Float3  cameraPosition;	// �J�������W
	};

	// �萔�o�b�t�@�p�f�[�^�\���� (�X�L�j���O)
	struct ConstBufferDataSkin
	{
		Matrix4 bones[MAX_BONES];
	};

public:	// �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model"> ���f�� </param>
	void SetModel(FBXModel* model) { this->model = model; }

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();

protected:	// �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	// �萔�o�b�t�@ (�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

	// ���[�J���X�P�[��
	Vector3 scale = { 1,1,1 };
	// X,Y,Z������̃��[�J����]�p
	Vector3 rotation = { 0,0,0 };
	// ���[�J�����W
	Vector3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	Matrix4 matWorld = {};
	// ���f��
	FBXModel* model = nullptr;

public:
	// 1�t���[���̎���
	FbxTime frameTime;
	// �A�j���[�V�����J�n����
	FbxTime startTime;
	// �A�j���[�V�����I������
	FbxTime endTime;
	// ���ݎ��� (�A�j���[�V����)
	FbxTime currentTime;
	// �A�j���[�V�����Đ���
	bool isPlay = false;

public:	// �ÓI�����o�֐�
	// setter
	static void SetDevice(ID3D12Device* device) { FBXObject3D::device = device; }
	static void SetCamera(Camera* camera) { FBXObject3D::camera = camera; }
	static void SetCommandList(ID3D12GraphicsCommandList* commandList) { FBXObject3D::commandList = commandList; }
	static void CreateGraphicsPipeline();	// �p�C�v���C���̐���

private: // �ÓI�����o�ϐ�
	static ID3D12GraphicsCommandList* commandList;	//�R�}���h���X�g
	static ID3D12Device* device;	// �f�o�C�X
	static Camera* camera;			// �J����
	static ComPtr<ID3D12RootSignature> rootSigunature;
	static ComPtr<ID3D12PipelineState> pipelineState;
};

