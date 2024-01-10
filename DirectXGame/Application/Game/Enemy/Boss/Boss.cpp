/**
 * @file	Boss.cpp
 * @brief	ボスのデータ保持や初期化/更新/描画を行うもの
**/

#include "Boss.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "TimeData.h"

void Boss::Initialize()
{
	// ボスモデル読み込み
	bossModel_ = OBJModel::LoadFromOBJ("ICO");

	// ボスの行列初期化
	position_ = BOSS_DEAD_POSITION_;
	scale_ = {RADIUS_, RADIUS_, RADIUS_};
	InitializeObject3D();

	// 変数初期化
	isAlive_ = true;

	// 敵機モデルと敵機オブジェクトを紐づけ
	objModel_ = &bossModel_;

	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, RADIUS_, 0 }), RADIUS_));
	collider_->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void Boss::Update()
{
	// 行列更新 必ず呼び出す
	UpdateObject3D();

	if (hitPoints_ <= deadHP_)
	{
		isAlive_ = false;
		position_ = BOSS_DEAD_POSITION_;
	}
}

void Boss::Draw()
{
	// オブジェ描画
	DrawObject3D();
}

void Boss::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	hitPoints_--;
}

void Boss::Spawn()
{
	position_ = INIT_BOSS_POSITION_;
}
