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
	Float3 pos;
	Float2 scale;
	Float4 color;
};

struct Particle
{
	Float3 position = {};	// ���W
	Float3 velocity = {}; // ���x
	Float3 accel = {};	// �����x
	int frame = 0;			// ���݃t���[��
	int num_frame = 0;		// �I���t���[��
	Float4 color = { 1,1,1,1 };

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
	ID3D12Resource* constBuffTransform;
	
	// ���_�f�[�^�Z�b�g
	const int vertexCount = 1024;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	Vertex* vertMap = nullptr;

	// �萔�o�b�t�@�}�b�v (�s��p)
	ParticleConstBufferDataTransform* constMapTransform;

	// �A�t�B���ϊ����
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

public:
	static ComPtr<ID3D12RootSignature> rootSignature; 	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> pipelineState;	// �p�C�v���C��

	// �p�C�v���C���쐬
	static void CreatePipeline();

	// particle������
	void InitializeParticle();

	// particle�X�V����
	void UpdateParticle();

	// particle�`�揈��
	void DrawParticle(TextureIndex index);

	// �R���e�i�ɒǉ��������
	void Add(int life, Float3 position, Float3 velocity, Float3 accel, float start_scale, float end_scale);

private:
	int activeCount = 0;
	forward_list<Particle> particles;
};