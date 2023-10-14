#pragma once
#include "TextureManager.h"
#include "Common.h"

class OBJModel
{
public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		Vector3 pos;	// xyz座標
		Vector3 normal;// 法線ベクトル 
		Vector2 uv;	// uv座標
	};

	// マテリアル
	struct Material
	{
		std::string name;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		float alpha;

		TextureIndex index = static_cast<TextureIndex>(-1);

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
		Vector3 ambient;
		float pad1;

		Vector3 diffuse;
		float pad2;

		Vector3 specular;
		float alpha;

		Vector4 color; // 色(RGBA)
	};

	Material material_;

public:
	// コンストラクタ
	OBJModel();

public:
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	// OBJファイルから３Dモデル読み込み
	static OBJModel LoadFromOBJ(const std::string& modelName);
	// material読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	// バッファ生成
	void CreateBuffers();
	// 描画処理
	void Draw();

private:
	// OBJ読み込み
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