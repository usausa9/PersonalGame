#pragma once
#include "Common.h"

class TimeData
{
private:
	// �ő厞��
	float maxTime = 0;

	// ���ݎ���
	float nowTime = 0;

	// 0.f - 1.f�ɕϊ�����f�[�^
	float timeRate = 0;

	// �^�C�}�[�̃A�N�e�B�u
	bool isActive = false;

public:
	/// <summary>
	/// �^�C�}�[�̊J�n
	/// </summary>
	/// <param name="allFrame">�S�̃t���[����</param>
	void Start(float allFrames);

	// �X�V����
	void Update();

	// ���[�g�̃Q�b�^�[
	float GetTimeRate() { return timeRate; }
};

