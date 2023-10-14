#pragma once

/**
 * @file	CollisionInfo.h
 * @brief	衝突情報受け取り
**/

#include "Vector3.h"

class Object3D;
class BaseCollider;

struct CollisionInfo
{
public:
	CollisionInfo(Object3D* object, BaseCollider* collider, const Vector3& inter)
	{
		this->object_ = object;
		this->collider_ = collider;
		this->inter_ = inter;
	}

	// 衝突相手のオブジェクト
	Object3D* object_ = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider_ = nullptr;
	// 衝突点
	Vector3 inter_;
};