#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Float2.h"
#include "Float3.h"
#include "Float4.h"

#include "Matrix4.h"

#include <cmath>

namespace UsaMath
{
	const float u_PI = 3.141593f;

	inline float DegreesToRadians(float Degrees)
	{
		return Degrees * (u_PI / 180.0f);
	}

	inline float RadiansToDegrees(float Radians)
	{
		return Radians * (180.0f / u_PI);
	}
}