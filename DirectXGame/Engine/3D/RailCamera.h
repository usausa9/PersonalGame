#pragma once
#include "Camera.h"
#include "Object3D.h"

class RailCamera
{
public:	// メンバ関数
	// デストラクタ
	~RailCamera();

	// 初期化処理
	void Initialize(const Vector3& pos, const Vector3& rot);

	// 更新処理
	void Update();

private: // メンバ変数
	// ワールド変換データ
	Object3D* worldTransform = nullptr;

	// カメラ
	Camera* camera = nullptr;
};

