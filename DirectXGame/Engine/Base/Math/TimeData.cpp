#include "TimeData.h"

#include <cassert>
#include <algorithm>
#include <functional>

void TimeData::Start(float allFrames)
{
	// ����������ő�t���[���̃Z�b�g
	maxTime = allFrames;

	// ���ݎ��Ԃ̏�����
	nowTime = 0.0f;

	// �^�C�}�[�����true��
	isActive = true;
}

void TimeData::Update()
{
	// �A�N�e�B�u���łȂ���΍X�V���Ȃ�
	if (!isActive) 
	{
		return;
	}

	// ���ݎ��Ԃ����Z
	nowTime += 1.0f;

	// 0.f - 1.f�ɕϊ�
	timeRate = nowTime / maxTime;

	// 1���z����Ȃ�1��, �^�C�}�[��~
	timeRate = min(timeRate, 1.f);
	if (timeRate == 1.f) 
	{
		isActive = false;
	}
}