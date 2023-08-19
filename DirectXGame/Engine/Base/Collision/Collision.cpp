#include "Collision.h"

#include <algorithm>

// "��" �� "��" �� �����蔻��
bool Collision::Col_SphereToSphere(const Sphere& s1, const Sphere& s2, float* distance, Vector3* inter)
{
	Vector3 p1, p2;
	float r1, r2;

	p1 = s1.position;
	p2 = s2.position;
	r1 = s1.radius;
	r2 = s2.radius;

	float dis = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z);
	float rr = (r1 + r2) * (r1 + r2);

	if (distance) 
	{
		*distance = dis;
	}

	if (dis > rr) 
	{
		return false;
	}

	if (inter) 
	{
		*inter = (p1 + p2) / 2.0f;
	}

	return true;
}

// "��" �� "����" �� �����蔻��
bool Collision::Col_SphereToPlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	// ���W�n�̒��_���狅�̒��S���W�ւ̋���
	float distV = sphere.position.Dot(plane.normal);

	// ���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - plane.distance;

	// �����̐�Βl�����a���傫����΂������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) return false;

	// �^����_���Z�o
	if (inter) 
	{
		// ���ʏ�̍ŋߐړ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.position;
	}

	return true;
}

// "�_" �� "�O�p�`" �� �ŋߐړ_�����߂�
void Collision::ClosestPtPointToTriangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point �� p0 �̊O���̒��_�̈�̒��ɂ��邩�ǂ������`�F�b�N
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	Vector3 p0_p2 = triangle.p2_ - triangle.p0_;
	Vector3 p0_pt = point - triangle.p0_;

	float d1 = p0_p1.Dot(p0_pt);
	float d2 = p0_p2.Dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f) 
	{
		*closest = triangle.p0_;
		return;
	}

	// point �� p1 �̊O���̒��_�̈�ɂ��邩�ǂ������`�F�b�N
	Vector3 p1_pt = point - triangle.p1_;

	float d3 = p0_p1.Dot(p1_pt);
	float d4 = p0_p2.Dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3) 
	{
		*closest = triangle.p1_;
		return;
	}

	// point �� p0_p1 �̕ӗ̈�̒��ɂ��邩�ǂ������`�F�b�N���A����� point �� p0_p1 ��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) 
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0_ + v * p0_p1;
		return;
	}

	// point �� p2 �̊O���̒��_�̈�̒��ɂ��邩�ǂ������`�F�b�N
	Vector3 p2_pt = point - triangle.p2_;

	float d5 = p0_p1.Dot(p2_pt);
	float d6 = p0_p2.Dot(p2_pt);

	if (d6 >= 0.0f && d5 <= d6) 
	{
		*closest = triangle.p2_;
		return;
	}

	// point �� p0_p2 �̕ӗ̈�̒��ɂ��邩�ǂ������`�F�b�N���A����� point �� p0_p2 ��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) 
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0_ + w * p0_p2;
		return;
	}

	// point �� p1_p2 �̕ӗ̈�̒��ɂ��邩�ǂ������`�F�b�N���A����� point �� p1_p2 ��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) 
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1_ + w * (triangle.p2_ - triangle.p1_);
		return;
	}

	float denomination = 1.0f / (va + vb + vc);
	float v = vb * denomination;
	float w = vc * denomination;
	*closest = triangle.p0_ + p0_p1 * v + p0_p2 * w;
}

// "��" �� "�O�p�`" �� �����蔻��
bool Collision::Col_SphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	Vector3 p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPointToTriangle(sphere.position, triangle, &p);

	// �_p �� ���̒��S �̍����x�N�g��
	Vector3 v = p - sphere.position;

	// ���������߂�
	// �����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����
	float len = v.Dot(v);

	// ���ƎO�p�`�̋��������a�ȉ��Ȃ炠�����Ă��Ȃ�
	if (len > sphere.radius * sphere.radius) return false;
	// �^����_���v�Z
	if (inter) 
	{
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}

	return true;
}

// "���C" �� "����" �� �����蔻��
bool Collision::Col_RayToPlane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl

	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = plane.normal.Dot(ray.dir);

	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return false; }

	// �n�_�ƌ��_�̋���(���ʂ̖@������)
	// �ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = plane.normal.Dot(ray.start);
	// �n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance;
	// �n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;

	// ��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0) return false;

	// ��������������
	if (distance) { *distance = t; }

	// ��_���v�Z
	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

// "���C" �� "�O�p�`" �� �����蔻��
bool Collision::Col_RayToTriangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal_;
	plane.distance = triangle.normal_.Dot(triangle.p0_);

	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!Col_RayToPlane(ray, plane, distance, &interPlane)) { return false; }

	// ���C�ƕ��ʂ��������Ă���̂ŁA�����ƌ�_���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl
	Vector3 m;

	// ��p0_p1�ɂ���
	Vector3 pt_p0 = triangle.p0_ - interPlane;
	Vector3 p0_p1 = triangle.p1_ - triangle.p0_;
	m = pt_p0.Cross(p0_p1);
	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(triangle.normal_) < -epsilon) { return false; }

	// ��p1_p2�ɂ���
	Vector3 pt_p1 = triangle.p1_ - interPlane;
	Vector3 p1_p2 = triangle.p2_ - triangle.p1_;
	m = pt_p1.Cross(p1_p2);
	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(triangle.normal_) < -epsilon) { return false; }

	// ��p2_p0�ɂ���
	Vector3 pt_p2 = triangle.p2_ - interPlane;
	Vector3 p2_p0 = triangle.p0_ - triangle.p2_;
	m = pt_p2.Cross(p2_p0);
	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(triangle.normal_) < -epsilon) { return false; }

	// �����̏ꍇ�A�������Ă���
	if (inter) 
	{
		*inter = interPlane;
	}

	return true;
}

// "���C" �� "��" �� �����蔻��
bool Collision::Col_RayToSphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.position;
	float b = m.Dot(ray.dir);
	float c = m.Dot(m) - sphere.radius * sphere.radius;
	// ray�̎n�_��sphere�̊O���ɂ���(c>0)�Aray��sphere���痣��Ă����������w���Ă���ꍇ(b>0)������Ȃ�
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	// ���̔��ʎ��̓��C�����𗣂�Ă��邱�ƂɈ�v
	if (discr < 0.0f) { return false; }

	// ���C�͋��ƌ������Ă���
	// ��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t��0�ɃN�����v
	t = std::max( t, 0.0f );

	if (distance) 
	{ 
		*distance = t; 
	}

	if (inter) 
	{ 
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

// "�~" �� "�~" �� �����蔻��
bool Collision::Col_CircleToCircle(const Circle& c1, const Circle& c2)
{
	float dis = (c2.pos.x - c1.pos.x) * (c2.pos.x - c1.pos.x) + (c2.pos.y - c1.pos.y) * (c2.pos.y - c1.pos.y);
	float rr = (c1.radius + c2.radius) * (c1.radius + c2.radius);

	if (dis > rr) 
	{
		return false;
	}

	return true;
}