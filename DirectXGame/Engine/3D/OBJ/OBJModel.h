#pragma once
#include "TextureManager.h"
#include "Common.h"

class OBJModel
{
public:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		Vector3 pos;	// xyz���W
		Vector3 normal;// �@���x�N�g�� 
		Vector2 uv;	// uv���W
	};

	// �}�e���A��
	struct Material
	{
		std::string name;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
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
		Vector3 ambient;
		float pad1;

		Vector3 diffuse;
		float pad2;

		Vector3 specular;
		float alpha;

		Vector4 color; // �F(RGBA)
	};

	Material material_;

public:
	OBJModel();

public:
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	// OBJ�t�@�C������RD���f���ǂݍ���
	static OBJModel LoadFromOBJ(const std::string& modelName);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void CreateBuffers();

	void Draw();

private:
	void LoadFromOBJInternal(const std::string& modelName);

private:
	std::vector<unsigned short> indices_;
	std::vector<VertexPosNormalUv> vertices_;

	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};
};