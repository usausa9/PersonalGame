#pragma once

/**
 * @file	FBXModel.h
 * @brief	FBXモデルの描画やバッファ生成を行うもの
**/

#include "TextureManager.h"
#include "Common.h"

#include <DirectXTex.h>
#include <fbxsdk.h>

struct Node
{
	// 名前
	string name_;

	// ローカルスケール
	Vector4 scaling_ = { 1,1,1,0 };
	// ローカル回転角
	Vector4 rotation_ = { 0,0,0,0 };
	// ローカル移動
	Vector4 translation_ = { 0,0,0,1 };
	// ローカル変形行列
	Matrix4 transform_;
	// グローバル変形行列
	Matrix4 globalTransform_;

	// 親ノード
	Node* parent_ = nullptr;
};

class FBXModel
{
private:
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 定数
	// ボーンインデックスの最大数
	static const int sMAX_BONE_INDICES_ = 4;

public:
	// フレンドクラス
	friend class FBXLoader;

	// ボーン構造体
	struct Bone
	{
		// 名前
		std::string name_;
		// 初期姿勢の逆行列
		Matrix4 invInitialPose_;
		// クラスター (FBX側のボーン情報)
		FbxCluster* fbxCluster_;
		// コンストラクタ
		Bone(const std::string& name)
		{
			this->name_ = name;
		}
	};

	// 頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		Vector3 pos;		// xyz座標
		Vector3 normal;	// 法線ベクトル 
		Vector2 uv;		// uv座標
		UINT boneIndex[sMAX_BONE_INDICES_];
		float boneWeight[sMAX_BONE_INDICES_];
	};

private:
	// モデル名
	string name_;
	// ノード配列
	vector<Node> nodes_;
	// メッシュを持つノード
	Node* meshNode_ = nullptr;
	// 頂点データ配列
	vector<VertexPosNormalUvSkin> vertices_;
	// 頂点インデックス配列
	vector<unsigned short> indices_;
	// アンビエント係数
	Vector3 ambient_ = { 1,1,1 };
	// ディフューズ係数
	Vector3 diffuse_ = { 1,1,1 };
	// テクスチャメタデータ
	TexMetadata metadata_ = {};
	// スクラッチイメージ
	ScratchImage scratchImg_ = {};
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_ = nullptr;
	// ボーン配列
	std::vector<Bone> bones_;
	// FBXシーン
	FbxScene* fbxScene_ = nullptr;

public:
	// デストラクタ
	~FBXModel();

	// バッファ生成
	void CreateBuffers(ID3D12Device* device);

	// 描画
	void Draw(ID3D12GraphicsCommandList* commandList);

	// モデルの変形行列取得
	const Matrix4& GetModelTransform() { return meshNode_->globalTransform_; }

	// getter
	std::vector<Bone>& GetBones() { return bones_; }

	// getter
	FbxScene* GetFbxScene() { return fbxScene_; }
};