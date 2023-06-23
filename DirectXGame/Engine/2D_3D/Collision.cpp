#include "Collision.h"

bool Collision::Col_Sphere_2_Plane
(const Sphere& sphere, const Plane& plane)
{
	// ���_���狅�̒��S���W
	float distV = sphere.center.Dot(plane.normal);

	// ���ʂ̌��_�������Z (���ʂƋ��̒��S�Ƃ̋���)
	float dist = distV - plane.distance;

	// �����̐�Βl�����a�����傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	return true;
}
