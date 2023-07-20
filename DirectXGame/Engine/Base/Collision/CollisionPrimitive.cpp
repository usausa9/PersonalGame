#include "CollisionPrimitive.h"

void Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p1 - p0;
	Vector3 p0_p2 = p2 - p0;

	// �O�ς��琂���ȃx�N�g�����Z�o
	normal = p0_p1.Cross(p0_p2);
	normal.Normalize();
}