#include "CollisionPrimitive.h"

void Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p1_ - p0_;
	Vector3 p0_p2 = p2_ - p0_;

	// �O�ς��琂���ȃx�N�g�����Z�o
	normal_ = p0_p1.Cross(p0_p2);
	normal_.Normalize();
}