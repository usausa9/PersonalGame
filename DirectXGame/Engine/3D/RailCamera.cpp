#include "RailCamera.h"
#include "Input.h"

using namespace Input;

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

	Vector3 start{ 0,0,-0 };
	Vector3 p1{ -10,20,-30 };
	Vector3 p2{ 10,-20,30 };
	Vector3 end{ 30,0,-50 };

	std::vector<Vector3> points{ start,p1,p2,end };

	spline.SetPositions(points);

}

void RailCamera::Update()
{
	if (Key::Trigger(DIK_O)) 
	{
		spline.MoveStart(300.0f, true);
	}

	// 曲線のアップデート
	spline.Update();

	// ワールド行列の座標の数値を加算
	// スプライン曲線に沿って移動
	worldTransform->position = spline.GetNowPosition();
	//worldTransform->position += { 0,0,0.15f };

	// ワールド行列の回転の数値を加算
	worldTransform->rotation += { 0, 0, 0 };
	
	// ワールド行列の行列再計算
	worldTransform->UpdateObject3D();

	// レールカメラには常にワールド座標を代入
	camera->position = {
		worldTransform->matWorld.m[3][0],
		worldTransform->matWorld.m[3][1],
		worldTransform->matWorld.m[3][2]};

	// ワールド前方ベクトル
	Vector3 forward = {0, 0, 1};

	// レールカメラの回転を反映
	forward = Matrix4::Transform(forward, worldTransform->matWorld);
	
	// 視点+前方ベクトルで注視点
	camera->target = camera->position + forward;
	
	// 上方向ベクトル
	Vector3 up = {0, 1, 0};
	
	// カメラの上方向をforwardと同様に変換
	camera->up = Matrix4::Transform(up, worldTransform->matWorld);
	
	// カメラ更新
	camera->Update();

}
