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
	void Update(const Matrix4& cameraMatrix);

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
	bool IsAlive()const { return isAlive_; }

	// �����蔻��󋵂��擾
	static bool IsOnCol() { return sIsOnCol_; }

private: // �G�@�̃����o�ϐ�
	// �G�@���f��, �G�@�I�u�W�F�N�g
	OBJModel enemyModel_;

	// �G�̋O��
	SplineCurve trajectory_ = {};

	// �����t���O
	bool isAlive_ = true;
	// �����蔻��t���O
	static bool sIsOnCol_;
};