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
	const float distanceReticle = 40.0f;

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
	void SetParent(Object3D* object) { parent = object; }

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
	OBJModel playerModel;

	OBJModel reticleModel;
	Object3D reticleObj;
	TextureIndex reticleTex;
	unique_ptr<Sprite> reticleSp = nullptr;

	TextureIndex aTex;
	unique_ptr<Sprite> aSp = nullptr;

	PlayerState state;

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };
	Vector3 rev = { 0,0,0 };

	// ���@�̈ړ��X�s�[�h 
	float velocity = 0.5f;
	const float formerlySpeed = 0.5f;
	const float speedUpRate = 0.06f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove = 0.7f;
	// ���@�̈ړ��͈͐����p
	const Vector2 kMoveLimit = { 32.f, 17.f };

	// ���@�̉�]�� 
	const float rotateRev = 0.03f;
	// ���@Y��]��
	const float kYRotate = 0.85f;
	// ���@�̉�]�͈͐����p
	const Vector3 kRevLimit = { 0.3f, 0.f, 0.3f };	

	// ���e�B�N���֘A
	float reticleSpd = 6.0f;
	const float kYMoveReticle = 0.7f;
	const float reticleRadius = 340.f;

	float kReticleSpd = 6.0f / 0.65f;


private: // ���@�e�̃����o�ϐ�
	// ���e�B�N���̍��W�f�[�^
	Vector2 reticlePos = { 640, 360 };

	// ���@�e�̃��X�g
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// ���@�e�̃��f��
	OBJModel bulletModel;
	
	// ���@�e�̃X�s�[�h
	const float kBulletSpeed = 2.2f;

	// �e���˂̊Ԋu�p
	TimeData shotTimeData = {};
	const float shotDelay = 1.f;
	const float shotInterval = 12.f;
};
