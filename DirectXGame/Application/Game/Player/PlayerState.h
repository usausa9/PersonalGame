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

private:
	// �p���[�A�b�v�A�C�e���֘A
	array<uint8_t, 5> powerUpSelect = { 0, 0, 0, 0, 0 };	// ���݂̑I�𓙕`��p
	array<uint8_t, 5> powerUpStatus = { 0, 0, 0, 0, 0 };	// �v���C���[�̋������
	uint8_t nowSelect = uint8_t(State::_NONE);

	bool isPowerUpAble = false;
	

private:	// �`��p
	// �e�N�X�`���֘A
	static const uint8_t powerUpNum = 5;
	static const uint8_t texNum = 12;
	static const uint8_t spriteNum = 20;
	array<TextureIndex, texNum> stateSelectTex = {};
	array<unique_ptr<Sprite>, spriteNum> stateSprite = {
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, 
		nullptr, nullptr, nullptr, nullptr, nullptr, };

	const float scaleTexUI = 1.5f;
	const Vector2 spriteBase = { 337, 667 };
	static const uint8_t spriteSize = 150;

};