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
	void Initialize(bool bulletState, OBJModel* model, const Vector3& pos, const Vector3& velocity);

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
	Vector3 velocity_ = {};

	// ����<frame>
	static const int32_t s_kLIFE_TIME_ = 60 * 5;	// 5�b
	
	// ���Ń^�C�}�[
	int32_t deathTimer_ = s_kLIFE_TIME_;

	// ���Ńt���O
	bool isDead_ = false;

	// �f�o�b�O�p
	static bool isDrawSp_;

	// ���@�����ɉ������T�C�Y�p�����[�^
	const float SMALL_SIZE_ = 1.5f;
	const float EXPAND_SIZE_ = 2.2f;
	const float SMALL_COLLIDER_SIZE_ = 2.5f;
	const float EXPAND_COLLIDER_SIZE_ = 4.f;
	const Vector3 SMALL_SCALE_ = { SMALL_SIZE_, SMALL_SIZE_, SMALL_SIZE_};
	const Vector3 EXPAND_SCALE_ = { EXPAND_SIZE_, EXPAND_SIZE_, EXPAND_SIZE_ };

public:
	// �O���Q�Ɨp��Getter
	bool IsDead() const { return isDead_; }
	
	bool IsDrawSP() const { return isDrawSp_; }
};