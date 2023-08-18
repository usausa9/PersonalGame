#pragma once
#include "Common.h"
#include "TextureManager.h"

#include "WinAPI.h"

// �萔�o�b�t�@�p �f�[�^�\���� (�}�e���A��)
struct SpriteConstBufferDataMaterial
{
	Float4 color;	// �F(RGBA)
	Matrix4 mat;	// 3D�ϊ��s��
};

class SpriteManager
{
public:
	// ������
	void Init();

	// �`��O����
	void PreDraw();

public: // �����o�ϐ�

	ComPtr<ID3DBlob> vsBlob_ = nullptr;		 // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob_ = nullptr;		 // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob_ = nullptr;	 // �G���[�I�u�W�F�N�g

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	// �p�C�v���C���X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

public:
	static Matrix4 SpriteProjection;
};