#pragma once
#include "Common.h"
#include "Vector3.h"
#include "OBJModel.h"
#include "Camera.h"

// �萔�o�b�t�@�p�f�[�^�\���� (3D�ϊ��s��)
struct ParticleConstBufferDataTransform
{
	Matrix4 mat; // 3D�ϊ��s��
};

struct Vertex
{
	Vector3 pos;
	Vector2 scale;
	Vector4 color;
};

struct Particle
{
	Vector3 position = {};	// ���W
	Vector3 velocity = {}; // ���x
	Vector3 accel = {};	// �����x
	uint32_t frame = 0;			// ���݃t���[��
	uint32_t num_frame = 0;		// �I���t���[��
	Vector4 color = { 1,1,1,1 };

	float scale = 1.0f;

	float s_scale = 1.0f;
	float e_scale = 0.0f;

	void Update() 
	{
		float t = (float)frame / num_frame;
		float t2 = 1 - t;

		//color = { t2,0,t,t2 };
	};
};

class ParticleManager
{
public:
	// �萔�o�b�t�@ (�s��p)
	ID3D12Resource* constBuffTransform_;
	
	// ���_�f�[�^�Z�b�g
	const uint16_t VERTEX_COUNT_ = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	Vertex* vertMap_ = nullptr;

	// �萔�o�b�t�@�}�b�v (�s��p)
	ParticleConstBufferDataTransform* constMapTransform_;

	// �A�t�B���ϊ����
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };

public:
	static ComPtr<ID3D12RootSignature> sRootSignature_; 	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> sPipelineState_;	// �p�C�v���C��

	// �p�C�v���C���쐬
	static void CreatePipeline();

	// particle������
	void InitializeParticle();

	// particle�X�V����
	void UpdateParticle();

	// particle�`�揈��
	void DrawParticle(TextureIndex index);

	// �R���e�i�ɒǉ��������
	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);

private:
	uint16_t activeCount_ = 0;
	forward_list<Particle> particles_;
};