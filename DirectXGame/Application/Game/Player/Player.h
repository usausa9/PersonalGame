#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"

class Player
{
public:
	void Initialize();
	void Update(Object3D player);
	void Draw();

private:
	Vector3 Move();

private:
	// ÉÇÉfÉãì«Ç›çûÇ›
	OBJModel playerModel;
	Object3D player;

	Vector3 position = { 0,0,0 };
	Vector3 move = { 0,0,0 };
	float Velocity = 0.5f;
};
