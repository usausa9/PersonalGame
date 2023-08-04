#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "TimeData.h"

#include <list>

class Enemy : public Object3D
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

	/// <summary>
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

private: // �����o�֐�
	// ���͎󂯕t�� + �ړ�
	void Move();

private: // ���@�̃����o�ϐ�
	//TimeData tData = {};

	// ���@���f��, ���@�I�u�W�F�N�g
	OBJModel enemyModel;

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };

	// ���@�̈ړ��X�s�[�h 
	const float velocity = 0.65f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove = 0.7f;
};
