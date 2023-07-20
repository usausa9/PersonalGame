#pragma once

#include "CollisionPrimitive.h"

/// <summary>
/// �����蔻��w���p�[�N���X
/// </summary>
class Collision
{
public:
	/// <summary>
	/// "��" �� "��" �� �����蔻��
	/// </summary>
	/// <param name="s1">��1</param>
	/// <param name="s2">��2</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o��)</param>
	/// <returns>�Փ˂��Ă��邩</returns>
	static bool Col_SphereToSphere(const Sphere& s1, const Sphere& s2, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// "��" �� "����" �� �����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_(���ʏ�ł̐ړ_)</param>
	/// <returns>�������Ă��邩</returns>
	static bool Col_SphereToPlane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// "�_" �� "�O�p�`" �� �ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_(�o��)</param>
	static void ClosestPtPointToTriangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// "��" �� "�O�p�`" �� �����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩</returns>
	static bool Col_SphereToTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// "���C" �� "����" �� �����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="plane">����</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o��)</param>
	/// <returns>�������Ă��邩</returns>
	static bool Col_RayToPlane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// "���C" �� "�O�p�`" �� �����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o��)</param>
	/// <returns>�������Ă��邩</returns>
	static bool Col_RayToTriangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// "���C" �� "��" �� �����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o��)</param>
	/// <returns>�������Ă��邩�ǂ���</returns>
	static bool Col_RayToSphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// "�~" �� "�~" �� �����蔻��
	/// </summary>
	/// <param name="c1">�~1</param>
	/// <param name="c2">�~2</param>
	/// <returns></returns>
	static bool Col_CircleToCircle(const Circle& c1, const Circle& c2);
};