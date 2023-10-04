#pragma once
#include "UsaMath.h"

// �C�[�W���O�p
namespace Easing
{
	// t��0-1�Aexp�͎w��
	Vector2 Out(Vector2 start, Vector2 end, float t, float exp = 3);
	// t��0-1�Aexp�͎w��
	Vector2 In(Vector2 start, Vector2 end, float t, float exp = 3);
	// t��0-1
	Vector2 InOut(Vector2 start, Vector2 end, float t);

	// t��0-1�Aexp�͎w��
	float Out(float start, float end, float t, float exp = 3);
	// t��0-1�Aexp�͎w��
	float In(float start, float end, float t, float exp = 3);
	// t��0-1
	float InOut(float start, float end, float t);

	// t��0-1�Aexp�͎w���A�߂�l��0-1
	float Out(float t, float exp = 3);
	// t��0-1�Aexp�͎w���A�߂�l��0-1
	float In(float t, float exp = 3);
	// t��0-1
	float InOut(float t);
};