#pragma once
#include "OBJModel.h"
#include "Object3d.h"
class PlayerBullet
{
public:
	void Initialize(OBJModel* model, const Vector3& pos);

	void Update();

	void Draw();

private: 
	OBJModel ico;
	Object3D bulletObj;

};

