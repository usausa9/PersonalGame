#pragma once

/**
 * @file	UsaMath.h
 * @brief	自作数学関数を全て読み込めるもの/雑多処理
**/

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix4.h"

#include "Easing.h"

#include <cmath>

namespace UsaMath
{
	const float PI_ = 3.141593f;
	const float RADIANS_PI_ = 180.000000f;

	// Degreeをラジアンに
	inline float DegreesToRadians(float Degrees)
	{
		return Degrees * (PI_ / RADIANS_PI_);
	}
	
	// ラジアンがらDegreeに
	inline float RadiansToDegrees(float Radians)
	{
		return Radians * (RADIANS_PI_ / PI_);
	}
}