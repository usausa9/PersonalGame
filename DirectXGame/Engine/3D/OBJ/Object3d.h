#pragma once
#include "Common.h"
#include "UsaMath.h"
#include "OBJModel.h"
#include "Camera.h"

#include "CollisionInfo.h"
#include "CollisionManager.h"

class BaseCollider;

// 定数バッファ用データ構造体 (3D変換行列)
struct Object3DConstBufferDataTransform
{
	Matrix4 mat; // 3D変換行列
};

class Object3D
{
public: // メンバ変数
	// カメラ
	static Camera* sCamera_;

	// 定数バッファ (行列用)
	ID3D12Resource* constBuffTransform_ = nullptr;

	// 定数バッファマップ (行列用)
	Object3DConstBufferDataTransform* constMapTransform_ = nullptr;

	// アフィン変換情報
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };

	Vector3 GetWorldPosition() const { return { matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2] }; }

	// ワールド変換行列
	Matrix4 matWorld_ = Matrix4::Identity();

	// 親オブジェクトへのポインタ
	Object3D* parent_ = nullptr;

	// モデルのポインタ
	OBJModel* objModel_ = nullptr;

	// クラス名 (デバッグ用)
	const char* NAME_ = nullptr;

	// コライダー
	BaseCollider* collider_ = nullptr;

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
	const Matrix4& GetMatWorld() { return matWorld_; }

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