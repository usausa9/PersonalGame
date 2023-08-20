#include "RailCamera.h"
#include "Input.h"

using namespace Input;

RailCamera::~RailCamera()
{
	delete worldTransform_;
	delete camera_;
}

void RailCamera::Initialize(const Vector3& pos, const Vector3& rot)
{
	worldTransform_ = new Object3D();
	camera_ = new Camera();

	// ワールド行列の初期設定
	worldTransform_->InitializeObject3D();
	worldTransform_->position_ = pos;
	worldTransform_->rotation_ = rot;

	// ビュー行列の初期化
	camera_->Initialize();

	// スプライン曲線の制御点
	Vector3 start{ 0,0,0 };
	Vector3 p1{ 100,250,150 };
	Vector3 p2{ 0,0,200 };
	Vector3 end{ 100,250,350 };

	std::vector<Vector3> points{ start,p1,p2,end };

	spline_.SetPositions(points);
}

void RailCamera::Update()
{
	// スプライン曲線によって動かす
	if (Key::Trigger(DIK_O))
	{
		spline_.MoveStart(2800.0f, true);
	}

	// 曲線のアップデート
	spline_.Update();

	// ワールド行列の座標の数値を加算
	// スプライン曲線に沿って移動
	worldTransform_->position_ = spline_.GetNowPosition();

	// ワールド行列の回転の数値を加算
	worldTransform_->rotation_ += { 0, 0, 0 };

	// ワールド行列の行列再計算
	worldTransform_->UpdateObject3D();

	// レールカメラには常にワールド座標を代入
	camera_->position_ = {
		worldTransform_->matWorld_.m[3][0],
		worldTransform_->matWorld_.m[3][1],
		worldTransform_->matWorld_.m[3][2] };

	// ワールド前方ベクトル
	Vector3 forward = { 0, 0, 1 };

	// レールカメラの回転を反映
	forward = Matrix4::Transform(forward, worldTransform_->matWorld_);

	// 視点+前方ベクトルで注視点
	camera_->target_ = camera_->position_ + forward;

	// 上方向ベクトル
	Vector3 up = { 0, 1, 0 };

	// カメラの上方向をforwardと同様に変換
	camera_->up_ = Matrix4::Transform(up, worldTransform_->matWorld_);

	// カメラ更新
	camera_->Update();
}