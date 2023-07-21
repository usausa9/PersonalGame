#pragma once
#include "UsaMath.h"
#include "Input.h"
#include "Object3d.h"
#include "PlayerBullet.h"
#include "Sprite.h"

class Player
{
public:
	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
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
	// ���͎󂯕t�� + �ړ�
	void Move();

	// ���@�e����
	void Shot();

	// ���e�B�N���̃A�b�v�f�[�g
	void reticleUpdate();

private: // ���@�̃����o�ϐ�
	// ���@���f��
	OBJModel playerModel;
	OBJModel bulletModel;
	Object3D playerObj;

	// ���e�B�N�����f��, �X�v���C�g
	OBJModel reticleModel;
	Object3D reticleObj;
	TextureIndex reticleTex;
	unique_ptr<Sprite> reticleSp = nullptr;

	// ���@�̈ړ��p
	Vector3 move = { 0,0,0 };
	const float Velocity = 0.65f;
	const float kYMove = 0.7f;

	// �͈͐����p
	const Vector2 kMoveLimit = { 25.f, 13.f };

private: // ���@�ȊO�̃����o�ϐ�
	// ���e�B�N���̍��W�f�[�^
	Vector2 reticlePos = { 640, 360 };

	// ���@�e�̃��X�g
	std::list<unique_ptr<PlayerBullet>> bullets = {};

	// ���@�e�̃��f��
	OBJModel bulletModel;
	
	// ���@�e�̃X�s�[�h
	const float kBulletSpeed = 0.7f;
};
