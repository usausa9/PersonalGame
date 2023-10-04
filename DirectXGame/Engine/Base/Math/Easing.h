#pragma once
#include "UsaMath.h"

// イージング用
namespace Easing
{
	// tは0-1、expは指数
	Vector2 Out(Vector2 start, Vector2 end, float t, float exp = 3);
	// tは0-1、expは指数
	Vector2 In(Vector2 start, Vector2 end, float t, float exp = 3);
	// tは0-1
	Vector2 InOut(Vector2 start, Vector2 end, float t);

	// tは0-1、expは指数
	float Out(float start, float end, float t, float exp = 3);
	// tは0-1、expは指数
	float In(float start, float end, float t, float exp = 3);
	// tは0-1
	float InOut(float start, float end, float t);

	// tは0-1、expは指数、戻り値は0-1
	float Out(float t, float exp = 3);
	// tは0-1、expは指数、戻り値は0-1
	float In(float t, float exp = 3);
	// tは0-1
	float InOut(float t);
};