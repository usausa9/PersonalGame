/**
 * @file	Boss.cpp
 * @brief	ボスのデータ保持や初期化/更新/描画を行うもの
**/

#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "TimeData.h"

void Boss::Initialize(Vector3 pos)
{
	// ボスモデル読み込み
	bossModel_ = OBJModel::LoadFromOBJ("");

	// ボスの行列初期化
	InitializeObject3D();

	// 敵機モデルと敵機オブジェクトを紐づけ
	objModel_ = &bossModel_;

	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, RADIUS_, 0 }), RADIUS_));
	collider_->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void Boss::Update(const Matrix4& cameraMatrix)
{
	// 行列更新 必ず呼び出す
	UpdateObject3D();
}

void Boss::Draw()
{
	// オブジェ描画
	DrawObject3D();
}

void Boss::OnCollision(const CollisionInfo& info)
{
	hitPoints_--;
}
