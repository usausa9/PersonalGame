#pragma once

#include "UsaMath.h"

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	// 座標
	Vector3 position = { 0,0,0 };

	// 半径
	float radius = 1.0f;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	// 法線ベクトル
	Vector3 normal = { 0,1,0 };

	// 原点(0,0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 法線付き三角形(時計回りが表面)
/// </summary>
class Triangle
{
public:
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;

	// 法線
	Vector3 normal_;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// レイ(半直線)
/// </summary>
struct Ray
{
	// 始点座標
	Vector3 start{ 0,0,0 };

	// 方向
	Vector3 dir{ 1,0,0 };
};

/// <summary>
/// 円(2D)
/// </summary>
struct Circle
{
	// 中心座標
	Vector2 pos{ 0,0 };

	// 半径
	float radius = 0.0f;
};