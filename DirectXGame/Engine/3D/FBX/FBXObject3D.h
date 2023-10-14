#pragma once
#include "FBXModel.h"
#include "FbxLoader.h"
#include "Camera.h"
#include "Common.h"
#include "DirectXBase.h"

#pragma warning(push,0)
#include <Windows.h>
#pragma warning(pop)

class FBXObject3D
{
protected:	// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 定数
	// ボーンの最大数
	static const int MAX_BONES_ = 32;

public:	// サブクラス
	// 定数バッファ用データ構造体 (座標変換行列用)
	struct ConstBufferDataTransform
	{
		Matrix4 viewProjection_;	// ビュープロジェクション行列
		Matrix4 worldTransform_;	// ワールド行列
		Vector3 cameraPosition_;	// カメラ座標
	};

	// 定数バッファ用データ構造体 (スキニング)
	struct ConstBufferDataSkin
	{
		Matrix4 bones_[MAX_BONES_];
	};

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model"> モデル </param>
	void SetModel(FBXModel* model) { this->model_ = model; }

	/// <summary>
	/// 位置のセッター
	/// </summary>
	/// <param name=" 位置 "></param>
	void SetPosition(Vector3 position) { this->position_ = position; }

	/// <summary>
	/// 大きさのセッター
	/// </summary>
	/// <param name=" 大きさ "></param>
	void SetScale(Vector3 scale) { this->scale_ = scale; }

	/// <summary>
	/// 角度のセッター
	/// </summary>
	/// <param name=" 回転 "></param>
	void SetRotation(Vector3 rotation) { this->rotation_ = rotation * (UsaMath::PI_ / 180); }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw();

	/// <summary>
	/// アニメーション開始
	/// </summary>
	void PlayAnimation();

protected:	// メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform_;
	// 定数バッファ (スキン)
	ComPtr<ID3D12Resource> constBuffSkin_;

	// ローカルスケール
	Vector3 scale_ = { 1,1,1 };
	// X,Y,Z軸周りのローカル回転角
	Vector3 rotation_ = { 0,0,0 };
	// ローカル座標
	Vector3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	Matrix4 matWorld_ = {};
	// モデル
	FBXModel* model_ = nullptr;

public:
	// 1フレームの時間
	FbxTime frameTime_;
	// アニメーション開始時間
	FbxTime startTime_;
	// アニメーション終了時間
	FbxTime endTime_;
	// 現在時間 (アニメーション)
	FbxTime currentTime_;
	// アニメーション再生中
	bool isPlay_ = false;

public:	// 静的メンバ関数
	// setter
	static void SetDevice(ID3D12Device* device) { DirectXBase::GetInstance()->device_ = device; }
	static void SetCamera(Camera* camera) { FBXObject3D::camera_ = camera; }
	static void SetCommandList(ID3D12GraphicsCommandList* commandList) { FBXObject3D::commandList_ = commandList; }
	
	// パイプラインの生成
	static void CreateGraphicsPipeline();	

private: // 静的メンバ変数
	static ID3D12GraphicsCommandList* commandList_;	//コマンドリスト
	static Camera* camera_;			// カメラ
	static ComPtr<ID3D12RootSignature> rootSigunature_;
	static ComPtr<ID3D12PipelineState> pipelineState_;
};

