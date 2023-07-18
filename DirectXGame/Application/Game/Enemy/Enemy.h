#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"

#include <list>

class Enemy
{
public:	// �����o�֐�
	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �e�q�t���̃Z�b�^�[
	// void SetParent(Object3D* object) { enemyObj.parent = object; }

private: // �����o�֐�
	// ���͎󂯕t�� + �ړ�
	void Move();

private: // ���@�̃����o�ϐ�
	// ���@���f��, ���@�I�u�W�F�N�g
	OBJModel enemyModel;
	Object3D enemyObj;

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };

	// ���@�̈ړ��X�s�[�h 
	const float velocity = 0.65f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove = 0.7f;
};
