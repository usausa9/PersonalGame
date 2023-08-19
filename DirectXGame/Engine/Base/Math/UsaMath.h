#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix4.h"

#include <cmath>

namespace UsaMath
{
	const float PI_ = 3.141593f;

	inline float DegreesToRadians(float Degrees)
	{
		return Degrees * (PI_ / 180.0f);
	}

	inline float RadiansToDegrees(float Radians)
	{
		return Radians * (180.0f / PI_);
	}
}