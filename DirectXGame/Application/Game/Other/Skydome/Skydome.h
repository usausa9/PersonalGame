#pragma once
#include "Object3d.h"

class Skydome
{
public:
	// ������
	void Initialize();

	// 
	void Update();

	// �`��
	void Draw();

private:
	// �V�����f��, �V���I�u�W�F�N�g
	OBJModel skydomeModel_;
	Object3D skydomeObj_;
};