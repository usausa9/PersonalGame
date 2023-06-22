#pragma once
#include "TextureManager.h"
#include "Common.h"

class OBJModel
{
public:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	// xyz���W
		XMFLOAT3 normal;// �@���x�N�g�� 
		XMFLOAT2 uv;	// uv���W
	};

	// �}�e���A��
	struct Material
	{
		std::string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		float alpha;

		TextureIndex index = -1;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	// �萔�o�b�t�@�p �f�[�^�\���� (�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT3 ambient;
		float pad1;

		XMFLOAT3 diffuse;
		float pad2;

		XMFLOAT3 specular;
		float alpha;

		XMFLOAT4 color; // �F(RGBA)
	};

	Material material;

public:
	OBJModel();

public:
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	// OBJ�t�@�C������RD���f���ǂݍ���
	static OBJModel LoadFromOBJ(const std::string& modelName);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void CreateBuffers();

	void Draw();

private:
	void LoadFromOBJInternal(const std::string& modelName);

private:
	std::vector<unsigned short> indices;
	std::vector<VertexPosNormalUv> vertices;

	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
};