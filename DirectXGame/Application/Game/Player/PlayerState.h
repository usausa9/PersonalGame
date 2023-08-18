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
		_NONE,		// �ǂ���I�΂�Ă��Ȃ�
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
	uint8_t nowSelect_ = uint8_t(State::_NONE);
	
	const uint8_t maxSpeedUpNum_ = 5;
	const uint8_t maxExpandNum_ = 1;
	const uint8_t maxLaserNum_ = 1;
	const uint8_t maxOptionNum_ = 2;
	const uint8_t maxBarrierNum_ = 1;

private:// �`��p
	// �e�N�X�`���֘A
	static const uint8_t powerUpNum_ = 5;
	static const uint8_t texNum_ = 12;
	static const uint8_t spriteNum_ = 20;
	array<TextureIndex, texNum_> stateSelectTex_ = {};
	array<unique_ptr<Sprite>, spriteNum_> stateSprite_ = {
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, };

	const float scaleTexUI_ = 1.5f;
	const Vector2 spriteBase_ = { 337, 667 };
	static const uint8_t spriteSize_ = 150;
};