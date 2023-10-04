#include "Easing.h"

Vector2 Easing::Out(Vector2 start, Vector2 end, float t, float exp)
{
	Vector2 dif = end - start;
	dif *= Out(t, exp);
	return start + dif;
}

Vector2 Easing::In(Vector2 start, Vector2 end, float t, float exp)
{
	Vector2 dif = end - start;
	dif *= In(t, exp);
	return start + dif;
}

Vector2 Easing::InOut(Vector2 start, Vector2 end, float t)
{
	Vector2 dif = end - start;
	dif *= InOut(t);
	return start + dif;
}

float Easing::Out(float start, float end, float t, float exp)
{
	float dif = end - start;
	dif *= Out(t, exp);
	return start + dif;
}

float Easing::In(float start, float end, float t, float exp)
{
	float dif = end - start;
	dif *= In(t, exp);
	return start + dif;
}

float Easing::InOut(float start, float end, float t)
{
	float dif = end - start;
	dif *= InOut(t);
	return start + dif;
}

float Easing::Out(float t, float exp)
{
	float r = 1;
	float t2 = 1 - t;
	for (int i = 0; i < exp; i++)
	{
		r *= t2;
	}

	return 1 - r;
}

float Easing::In(float t, float exp)
{
	float r = 1;
	for (int i = 0; i < exp; i++)
	{
		r *= t;
	}
	return r;
}

float Easing::InOut(float t)
{
	return t * t * (3.f - 2.f * t);
}