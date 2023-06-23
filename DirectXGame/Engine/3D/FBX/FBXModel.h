#pragma once
#include "TextureManager.h"
#include "Common.h"

#include <DirectXTex.h>
#include <fbxsdk.h>

struct Node
{
	// ���O
	string name;

	// ���[�J���X�P�[��
	Vector4 scaling = { 1,1,1,0 };
	// ���[�J����]�p
	Vector4 rotation = { 0,0,0,0 };
	// ���[�J���ړ�
	Vector4 translation = { 0,0,0,1 };
	// ���[�J���ό`�s��
	Matrix4 transform;
	// �O���[�o���ό`�s��
	Matrix4 globalTransform;

	// �e�m�[�h
	Node* parent = nullptr;
};

class FBXModel
{
private:
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �萔
	// �{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

public:
	// �t�����h�N���X
	friend class FBXLoader;

	// �{�[���\����
	struct Bone
	{
		// ���O
		std::string name;
		// �����p���̋t�s��
		Matrix4 invInitialPose;
		// �N���X�^�[ (FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		// �R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	// ���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		Float3 pos;		// xyz���W
		Float3 normal;	// �@���x�N�g�� 
		Float2 uv;		// uv���W
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

private:
	// ���f����
	string name;
	// �m�[�h�z��
	vector<Node> nodes;
	// ���b�V�������m�[�h
	Node* meshNode = nullptr;
	// ���_�f�[�^�z��
	vector<VertexPosNormalUvSkin> vertices;
	// ���_�C���f�b�N�X�z��
	vector<unsigned short> indices;
	// �A���r�G���g�W��
	Float3 ambient = { 1,1,1 };
	// �f�B�t���[�Y�W��
	Float3 diffuse = { 1,1,1 };
	// �e�N�X�`�����^�f�[�^
	TexMetadata metadata = {};
	// �X�N���b�`�C���[�W
	ScratchImage scratchImg = {};
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff = nullptr;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV = nullptr;
	// �{�[���z��
	std::vector<Bone> bones;
	// FBX�V�[��
	FbxScene* fbxScene = nullptr;

public:
	// �f�X�g���N�^
	~FBXModel();

	// �o�b�t�@����
	void CreateBuffers(ID3D12Device* device);

	// �`��
	void Draw(ID3D12GraphicsCommandList* commandList);

	// ���f���̕ό`�s��擾
	const Matrix4& GetModelTransform() { return meshNode->globalTransform; }

	// getter
	std::vector<Bone>& GetBones() { return bones; }

	// getter
	FbxScene* GetFbxScene() { return fbxScene; }
};