#pragma once
/**
 * @file	RailCamera.h
 * @brief	レールカメラのデータ保持や初期化/更新/描画を行うもの
**/

#include "Camera.h"
#include "Object3D.h"
#include "Common.h"

#include "SplineCurve.h"

class RailCamera
{
private: // パラメーター
	// レールカメラの1周の時間
	const float CAMERA_LOOP_TIME_ = 2800.f;

	// レールカメラの制御点
	const Vector3 SPLINE_START_ = { 0,0,0 };
	const Vector3 SPLINE_P1_ = { 100,250,150 };
	const Vector3 SPLINE_P2_ = { 0,0,200 };
	const Vector3 SPLINE_END_ = { 100,250,350 };

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

	// 動いているかフラグ
	static bool isMove_;
};