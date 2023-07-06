#pragma once
#include "TextureManager.h"
#include "Common.h"

class OBJModel
{
public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		Float3 pos;	// xyz座標
		Float3 normal;// 法線ベクトル 
		Float2 uv;	// uv座標
	};

	// マテリアル
	struct Material
	{
		std::string name;
		Float3 ambient;
		Float3 diffuse;
		Float3 specular;
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

	// 定数バッファ用 データ構造体 (マテリアル)
	struct ConstBufferDataMaterial
	{
		Float3 ambient;
		float pad1;

		Float3 diffuse;
		float pad2;

		Float3 specular;
		float alpha;

		Float4 color; // 色(RGBA)
	};

	Material material;

public:
	OBJModel();

public:
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	// OBJファイルから３Dモデル読み込み
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