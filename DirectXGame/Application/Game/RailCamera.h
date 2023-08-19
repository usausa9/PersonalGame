#pragma once
#include "Camera.h"
#include "Object3D.h"
#include "Common.h"

#include "SplineCurve.h"

class RailCamera
{
public:	// メンバ関数
	// デストラクタ
	~RailCamera();

	// 初期化処理
	void Initialize(const Vector3& pos, const Vector3& rot);

	// 更新処理
	void Update();

	// オブジェクトのゲッター
	Object3D* GetObject3d()const { return worldTransform_; }

	// カメラのゲッター
	Camera* GetCamera()const { return camera_; }

private: // メンバ変数
	// ワールド変換データ
	Object3D* worldTransform_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// カメラ用スプライン曲線
	SplineCurve spline_;
};