#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "Camera.h"
#include "TimeData.h"
#include "PlayerState.h"

#include <list>

class Player : public Object3D
{
private:
	const float distanceReticle_ = 40.0f;

public:	// �����o�֐�
	// ����������
	void Initialize(Camera* camera);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// (�����)���e�B�N���̃h���[
	void DrawUI();

	// �e�q�t���̃Z�b�^�[
	void SetParent(Object3D* object) { parent_ = object; }

	/// <summary>
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

private: // �����o�֐�
	// ���e�B�N���̃A�b�v�f�[�g
	void reticleUpdate();

	// ���͎󂯕t�� + �ړ�
	void Move();

	// ���@�e����
	void Shot();

private: // ���@�̃����o�ϐ�
	// ���@���f��, ���@�I�u�W�F�N�g
	OBJModel playerModel_;

	OBJModel reticleModel_;
	Object3D reticleObj_;
	TextureIndex reticleTex_;
	unique_ptr<Sprite> reticleSp_ = nullptr;

	TextureIndex aTex_;
	unique_ptr<Sprite> aSp_ = nullptr;

	PlayerState state_;

	// ���@�̈ړ��p
	Vector3 move_ = { 0,0,0 };
	Vector3 rev_ = { 0,0,0 };

	// ���@�̈ړ��X�s�[�h 
	float velocity_ = 0.5f;
	const float formerlySpeed_ = 0.5f;
	const float speedUpRate_ = 0.06f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove_ = 0.7f;
	// ���@�̈ړ��͈͐����p
	const Vector2 kMoveLimit_ = { 32.f, 17.f };

	// ���@�̉�]�� 
	const float rotateRev_ = 0.03f;
	// ���@Y��]��
	const float kYRotate_ = 0.85f;
	// ���@�̉�]�͈͐����p
	const Vector3 kRevLimit_ = { 0.3f, 0.f, 0.3f };

	// ���e�B�N���֘A
	float reticleSpd_ = 6.0f;
	const float kYMoveReticle_ = 0.7f;
	const float reticleMoveLimit_ = 340.f;

	float kReticleSpd_ = 6.0f / 0.65f;

private: // ���@�e�̃����o�ϐ�
	// ���e�B�N���̍��W�f�[�^
	Vector2 reticlePos_ = { 640, 360 };

	// ���@�e�̃��X�g
	std::list<unique_ptr<PlayerBullet>> bullets_ = {};

	// ���@�e�̃��f��
	OBJModel bulletModel_;
	
	// ���@�e�̃X�s�[�h
	const float kBulletSpeed_ = 2.2f;

	// �e���˂̊Ԋu�p
	TimeData shotTimeData_ = {};
	const float shotDelay_ = 1.f;
	const float shotInterval_ = 12.f;
};
