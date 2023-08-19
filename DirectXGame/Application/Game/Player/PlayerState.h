#pragma once

#include "Common.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteManager.h"

using namespace std;

class PlayerState
{
public:
	enum class SelectState : uint8_t
	{
		NONE,		// �I�΂�Ă��Ȃ�
		SELECT,		// �I�΂�Ă���
		USED,		// �g�p�ς݂ł���I�΂�ĂȂ�
		USEDSELECT,	// �g�p�ς݂ł���I�΂�Ă���
	};

	enum class State : uint8_t
	{
		SPEEDUP,	// ����
		EXPAND,		// ���e�B�N�����傫���Ȃ�
		LASER,		// ���[�U�[��
		OPTION,		// �v���n�u��������
		BARRIER,	// �T��܂Ń_���[�W���󂯂���o���A
		NONESTATE,		// �ǂ���I�΂�Ă��Ȃ�
	};

public:
	// �摜�ǂݍ��� + �R�Â�
	void SetTexture();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void DrawUI();

	// �����󋵂��擾
	uint8_t SpeedUpNum() const { return powerUpStatus_[0]; }
	uint8_t ExpandNum()  const { return powerUpStatus_[1]; }
	uint8_t LaserNum()   const { return powerUpStatus_[2]; }
	uint8_t OptionNum()  const { return powerUpStatus_[3]; }
	uint8_t BarrierNum() const { return powerUpStatus_[4]; }

private:
	// �p���[�A�b�v�A�C�e���֘A
	array<uint8_t, 5> powerUpSelect_ = { 0, 0, 0, 0, 0 };	// ���݂̑I�𓙕`��p
	array<uint8_t, 5> powerUpStatus_ = { 0, 0, 0, 0, 0 };	// �v���C���[�̋������
	uint8_t nowSelect_ = uint8_t(State::NONESTATE);
	
	const uint8_t MAX_SPEEDUP_NUM_ = 5;
	const uint8_t MAX_EXPAND_NUM_ = 1;
	const uint8_t MAX_LASER_NUM_ = 1;
	const uint8_t MAX_OPTION_NUM_ = 2;
	const uint8_t MAX_BARRIER_NUM_ = 1;

private:// �`��p
	// �e�N�X�`���֘A
	static const uint8_t POWER_UP_NUM_ = 5;
	static const uint8_t TEX_NUM_ = 12;
	static const uint8_t SPRITE_NUM_ = 20;

	array<TextureIndex, TEX_NUM_> stateSelectTex_ = {};
	array<unique_ptr<Sprite>, SPRITE_NUM_> stateSprite_ = {
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, };

	const float SCALE_TEX_UI_ = 1.5f;
	const Vector2 SPRITE_BASE_POS_ = { 337, 667 };
	static const uint8_t s_SPRITE_SIZE_ = 150;
};