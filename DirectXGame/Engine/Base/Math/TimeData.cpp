#include "TimeData.h"

#include <cassert>
#include <algorithm>
#include <functional>

void TimeData::Start(float allFrames)
{
	// ����������ő�t���[���̃Z�b�g
	maxTime_ = allFrames;

	// ���ݎ��Ԃ̏�����
	nowTime_ = 0.0f;

	// �^�C�}�[�����true��
	isActive_ = true;
}

void TimeData::Update()
{
	// �A�N�e�B�u���łȂ���΍X�V���Ȃ�
	if (!isActive_) 
	{
		return;
	}

	// ���ݎ��Ԃ����Z
	nowTime_ += 1.0f;

	// 0.f - 1.f�ɕϊ�
	timeRate_ = nowTime_ / maxTime_;

	// 1���z����Ȃ�1��, �^�C�}�[��~
	timeRate_ = min(timeRate_, 1.f);
	if (timeRate_ == 1.f) 
	{
		isActive_ = false;
	}
}