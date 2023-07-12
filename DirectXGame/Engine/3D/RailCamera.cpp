#include "RailCamera.h"

RailCamera::~RailCamera()
{
	delete worldTransform;
	delete camera;
}

void RailCamera::Initialize(const Vector3& pos, const Vector3& rot)
{
	worldTransform = new Object3D();
	camera = new Camera();

	// ワールド行列の初期設定
	worldTransform->InitializeObject3D();
	worldTransform->position = pos;
	worldTransform->rotation = rot;

	// ビュー行列の初期化
	camera->Initialize();
}

void RailCamera::Update()
{
	// ワールド行列の座標の数値を加算
	worldTransform->position += { 0,1,5 };

	// ワールド行列の回転の数値を加算
	worldTransform->rotation += { 0.05f, 0.05f, 0.05f };
	
	// ワールド行列の行列再計算
	worldTransform->UpdateObject3D();

	//camera->position = worldTransform->position;
}
