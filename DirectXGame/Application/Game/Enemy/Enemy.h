#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "TimeData.h"
#include "SplineCurve.h"

#include <list>

class Enemy : public Object3D
{
public:	// �����o�֐�
	// ����������
	void Initialize(std::vector<Vector3>& points);

	// �X�V����
	void Update(const Matrix4& cameraMatrix, const Vector3& playerWorldPosition);

	// �`�揈��
	void Draw();

	// ��������
	void Spawn();

	/// <summary>
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	// �G�̐����󋵂��擾
	bool IsAlive()const { return isAlive; }

private: // �����o�֐�
	// ���͎󂯕t�� + �ړ�
	void Move();

private: // ���@�̃����o�ϐ�
	//TimeData tData = {};

	// ���@���f��, ���@�I�u�W�F�N�g
	OBJModel enemyModel;

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };

	// �G�̋O��
	SplineCurve trajectory = {};

	// ���@�̈ړ��X�s�[�h 
	const float velocity = 0.65f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove = 0.7f;

	// �����t���O
	bool isAlive = true;
};
