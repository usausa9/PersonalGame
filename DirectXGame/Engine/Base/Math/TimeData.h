#pragma once
#include "Common.h"

class TimeData
{
private:
	// �ő厞��
	float maxTime_ = 0;

	// ���ݎ���
	float nowTime_ = 0;

	// 0.f - 1.f�ɕϊ�����f�[�^
	float timeRate_ = 0;

	// �^�C�}�[�̃A�N�e�B�u
	bool isActive_ = false;

public:
	/// <summary>
	/// �^�C�}�[�̊J�n
	/// </summary>
	/// <param name="allFrame">�S�̃t���[����</param>
	void Start(float allFrames);

	// �X�V����
	void Update();

	// ���ݎ��Ԃ̃Q�b�^�[
	float GetTime() { return nowTime_; }

	// ���[�g�̃Q�b�^�[
	float GetTimeRate() { return timeRate_; }

	// 1 - ���[�g�̃Q�b�^�[
	float GetDisTimeRate() { return 1.f - timeRate_; }

	// �A�N�e�B�u�̃Q�b�^�[
	bool GetActive() { return isActive_; }
}; 