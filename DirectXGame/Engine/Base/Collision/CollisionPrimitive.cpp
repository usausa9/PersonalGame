/**
 * @file	CollisionPrimitive.cpp
 * @brief	当たり判定の形を定めるもの
**/

#include "CollisionPrimitive.h"

void Triangle::ComputeNormal()
{
	Vector3 p0_p1 = p1_ - p0_;
	Vector3 p0_p2 = p2_ - p0_;

	// 外積から垂直なベクトルを算出
	normal_ = p0_p1.Cross(p0_p2);
	normal_.Normalize();
}