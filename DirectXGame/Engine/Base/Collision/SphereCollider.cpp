#include "SphereCollider.h"

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const Matrix4& matWorld = object3d->GetMatWorld();

	// ���̃����o�ϐ����X�V
	Sphere::position.x = matWorld.m[3][0];
	Sphere::position.y = matWorld.m[3][1];
	Sphere::position.z = matWorld.m[3][2];
	Sphere::position + offset;
	Sphere::radius = radius;
}
