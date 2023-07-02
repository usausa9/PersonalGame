#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"

class Player
{
public:
	void Initialize();
	void Update(Object3D player);

private:
	Vector3 Move();

public:
	Vector3 position = { 0,0,0 };

private:
	Vector3 move = { 0,0,0 };
	float Velocity = 0.5f;
};
