#include "SphereCollider.h"

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const Matrix4& matWorld = object3d_->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::position.x = matWorld.m[3][0];
	Sphere::position.y = matWorld.m[3][1];
	Sphere::position.z = matWorld.m[3][2];
	Sphere::position + offset_;
	Sphere::radius = radius_;
}