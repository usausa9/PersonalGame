#pragma once
#include "Vector3.h"

class Object3D;
class BaseCollider;

/// <summary>
/// �Փˏ��
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(Object3D* object, BaseCollider* collider, const Vector3& inter)
	{
		this->object_ = object;
		this->collider_ = collider;
		this->inter_ = inter;
	}

	// �Փˑ���̃I�u�W�F�N�g
	Object3D* object_ = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider_ = nullptr;
	// �Փ˓_
	Vector3 inter_;
};