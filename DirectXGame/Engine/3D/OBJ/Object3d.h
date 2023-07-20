#pragma once
#include "Common.h"
#include "UsaMath.h"
#include "OBJModel.h"

#include "CollisionInfo.h"
#include"CollisionManager.h"

class BaseCollider;

// 定数バッファ用データ構造体 (3D変換行列)
struct Object3DConstBufferDataTransform
{
	Matrix4 mat; // 3D変換行列
};

class Object3D
{
public: // メンバ変数
	// 定数バッファ (行列用)
	ID3D12Resource* constBuffTransform = nullptr;

	// 定数バッファマップ (行列用)
	Object3DConstBufferDataTransform* constMapTransform = nullptr;

	// アフィン変換情報
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };

	Vector3 GetWorldPosition() const { return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] }; }

	// ワールド変換行列
	Matrix4 matWorld = Matrix4::Identity();

	// 親オブジェクトへのポインタ
	Object3D* parent = nullptr;

	// モデルのポインタ
	OBJModel* objModel = nullptr;

	// クラス名 (デバッグ用)
	const char* name = nullptr;

	// コライダー
	BaseCollider* collider = nullptr;

public:
	// コンストラクタ
	Object3D() = default;

	// デストラクタ
	virtual ~Object3D();

	// 3Dオブジェクト初期化
	virtual void InitializeObject3D();

	// 3Dオブジェクト更新処理
	virtual void UpdateObject3D();

	// 3Dオブジェクト描画処理
	virtual void DrawObject3D();

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const Matrix4& GetMatWorld() { return matWorld; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}
};