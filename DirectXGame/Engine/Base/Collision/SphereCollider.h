#pragma once

/**
 * @file	SphereCollider.h
 * @brief	球衝突判定オブジェクト
**/

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include "Vector3.h"

class SphereCollider : public BaseCollider, public Sphere
{
public:
	// コンストラクタ
	SphereCollider(Vector3 offset = { 0,0,0 }, float radius = 1.0f) :
		offset_(offset),
		radius_(radius)
	{
		// 球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	// 半径のセッター
	inline void SetRadius(float radius) { this->radius_ = radius; }

private:
	// オブジェクト中心からのオフセット
	Vector3 offset_;
	// 半径
	float radius_;
};