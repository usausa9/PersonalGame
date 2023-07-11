#pragma once
#include "OBJModel.h"
#include "Object3d.h"

class PlayerBullet
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���@�e ���f��</param>
	/// <param name="pos">���ˈʒu</param>
	void Initialize(OBJModel* model, const Vector3& pos, const Vector3& velocity);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private: 
	Object3D bulletObj;

	// ���@�e�̖��t���[���ړ���
	Vector3 velocity = {};

};