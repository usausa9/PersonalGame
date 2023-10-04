#include "PlayerBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

bool PlayerBullet::isDrawSp_ = false;

void PlayerBullet::Initialize(bool bulletState, OBJModel* model, const Vector3& pos, const Vector3& vel)
{
	// モデル紐づけ(仮引数で貰ってくる)
	objModel_ = model;

	// 行列更新
	position_ = pos;
	if (bulletState == true)
	{
		scale_ = SMALL_SCALE_;
	}
	else
	{
		scale_ = EXPAND_SCALE_;
	}
	InitializeObject3D();

	// 引数で移動量を受け取る
	velocity_ = vel;

	// コライダーの追加 (強化ステートによりけり弾の大きさを変更)
	float radius = 0.f;
	if (bulletState == true)
	{
		radius = EXPAND_COLLIDER_SIZE_;
	}
	else
	{
		radius = SMALL_COLLIDER_SIZE_;
	}
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);
}

void PlayerBullet::Update()
{
	isDrawSp_ = false;
	
	// 時間経過で消滅
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	// 移動量から座標を移動
	position_ += velocity_;

	// 行列更新
	UpdateObject3D();
}

void PlayerBullet::Draw()
{
	// オブジェクト描画
	DrawObject3D();
}

void PlayerBullet::OnCollision(const CollisionInfo& info)
{
	isDrawSp_ = true;
}