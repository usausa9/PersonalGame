#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "Camera.h"

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

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };

	// ���@�̈ړ��X�s�[�h 
	const float velocity = 0.65f;

	// ���@��X�ɑ΂��Ă�Y�X�s�[�h
	const float kYMove = 0.7f;

	// ���@�̈ړ��͈͐����p
	const Vector2 kMoveLimit = { 25.f, 13.f };

private: // ���@�e�̃����o�ϐ�
	// ���e�B�N���̍��W�f�[�^
	Vector2 reticlePos = { 640, 360 };

	// ���@�e�̃��X�g
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// ���@�e�̃��f��
	OBJModel bulletModel;
	
	// ���@�e�̃X�s�[�h
	const float kBulletSpeed = 0.7f;
};
