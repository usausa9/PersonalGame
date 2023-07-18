#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"

#include <list>

class Player
{
public:	// �����o�֐�
	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �e�q�t���̃Z�b�^�[
	void SetParent(Object3D* object) { playerObj.parent = object; }

private: // �����o�֐�
	// ���͎󂯕t�� + �ړ�
	void Move();

	// ���@�e����
	void Shot();

private: // ���@�̃����o�ϐ�
	// ���@���f��, ���@�I�u�W�F�N�g
	OBJModel playerModel;
	Object3D playerObj;

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };

	// ���@�̈ړ��X�s�[�h 
	const float velocity = 0.65f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove = 0.7f;

	// ���@�̈ړ��͈͐����p
	const Vector2 kMoveLimit = { 25.f, 13.f };

private: // ���@�e�̃����o�ϐ�
	// ���@�e�̃��X�g
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// ���@�e�̃��f��
	OBJModel bulletModel;
	
	// ���@�e�̃X�s�[�h
	const float kBulletSpeed = 0.7f;
};
