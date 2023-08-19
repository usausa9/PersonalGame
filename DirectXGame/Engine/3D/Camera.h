#pragma once
#include "Common.h"

// 定数バッファ用データ構造体 (3D変換行列)
struct CameraConstBufferData
{
	Matrix4 view; 
	Matrix4 projection;
	Matrix4 billboard;
	Vector3 position;
};

class Camera
{
public:
	~Camera();

public:
	// 定数バッファ (行列用)
	ID3D12Resource* constBuffCamera_ = nullptr;

	// 定数バッファマップ (行列用)
	CameraConstBufferData* constMapCamera_ = nullptr;

	// アフィン変換情報
	Vector3 target_ = { 0,0,0 };
	Vector3 position_ = { 0,0,-20 };

	// 3D用の射影変換
	Matrix4 matProjection_ = Matrix4::Identity();

	// ビュー変換行列
	Matrix4 matView_ = Matrix4::Identity();
	Vector3 up_ = { 0,1,0 };

	float nearZ_ = 0.1f;
	float farZ_ = 1500.0f;
	
	// ビルボード行列
	Matrix4 matBillboard_ = Matrix4::Identity();

	// 今使用しているカメラからのポインタ、カメラへのポインタ
	static Camera* sCurrentCamera;

	static void SetCurrentCamera(Camera* current);

	static Camera* GetCurrentCamera();

public:
	// カメラ初期化
	void Initialize();
	void Initialize(Vector3 position, Vector3 target, Vector3 up = {0, 1, 0});

	// カメラ更新処理
	void Update();

	// カメラセット
	void Set();

	// ビュープロジェクション
	Matrix4 GetViewProjection()const { return matView_ * matProjection_; }
	
	// カメラ座標取得
	Vector3 GetEye()const { return position_; }
};