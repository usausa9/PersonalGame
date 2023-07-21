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
	ID3D12Resource* constBuffCamera = nullptr;

	// 定数バッファマップ (行列用)
	CameraConstBufferData* constMapCamera = nullptr;

	// アフィン変換情報
	// Float3 rotation = { 0,0,0 };
	Vector3 target = { 0,0,0 };
	Vector3 position = { 0,0,-20 };

	// 3D用の射影変換
	Matrix4 matProjection = Matrix4::Identity();

	// ビュー変換行列
	Matrix4 matView = matView.Identity();
	Vector3 up = { 0,1,0 };

	float nearZ = 0.1f;
	float farZ = 1500.0f;
	
	// ビルボード行列
	Matrix4 matBillboard = Matrix4::Identity();

	// 今使用しているカメラからのポインタ、カメラへのポインタ
	static Camera* CurrentCamera;

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
	Matrix4 GetViewProjection()const { return matView * matProjection; }
	
	// カメラ座標取得
	Vector3 GetEye()const { return position; }
};