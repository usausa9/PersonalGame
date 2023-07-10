#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"

#include <list>

class Player
{
public:
	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Draw();

private:
	// ���͎󂯕t�� + �ړ�
	void Move();

	// ���@�e����
	void Shot();

private:
	// ���@�e
	//PlayerBullet* bullet = nullptr;
	//shared_ptr<PlayerBullet> bullet = nullptr;

	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// ���f���ǂݍ���
	OBJModel bulletModel;
	OBJModel playerModel;
	Object3D playerObj;

	Vector3 position = { 0,0,0 };
	Vector3 move = { 0,0,0 };
	const float Velocity = 0.65f;
	const float kYMove = 0.7f;

	// �͈͐����p
	const Vector2 kMoveLimit = { 25.f, 13.f };
};
