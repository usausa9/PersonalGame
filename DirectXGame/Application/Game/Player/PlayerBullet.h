#pragma once
#include "OBJModel.h"
#include "Object3D.h"
#include "Sprite.h"

class PlayerBullet : public Object3D
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���@�e ���f��</param>
	/// <param name="pos">���ˈʒu</param>
	void Initialize(OBJModel* model, const Vector3& pos, const Vector3& velocity);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	/// <summary>
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

private: 
	// ���@�e�̖��t���[���ړ���
	Vector3 velocity = {};

	// ����<frame>
	static const int32_t kLifeTime = 60 * 5;	// 5�b
	
	// ���Ń^�C�}�[
	int32_t deathTimer = kLifeTime;

	// ���Ńt���O
	bool isDead = false;

	// �f�o�b�O�p
	static bool isDrawSp;

public:
	// �O���Q�Ɨp��Getter
	bool IsDead() const { return isDead; }
	
	bool IsDrawSP() const { return isDrawSp; }
};