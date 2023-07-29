#include "PlayerBullet.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

bool PlayerBullet::isDrawSp = false;

void PlayerBullet::Initialize(OBJModel* model, const Vector3& pos, const Vector3& vel)
{
	// モデル紐づけ(仮引数で貰ってくる)
	objModel = model;

	// 行列更新,位置更新
	position = pos;
	InitializeObject3D();

	// 引数で移動量を受け取る
	velocity = vel;

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void PlayerBullet::Update()
{
	isDrawSp = false;
	
	// 時間経過で消滅
	if (--deathTimer <= 0)
	{
		isDead = true;
	}

	// 移動量から座標を移動
	position += velocity;

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
	isDrawSp = true;
}