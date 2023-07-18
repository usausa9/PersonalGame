#include "SplineCurve.h"

void SplineCurve::MoveStart(float allTime, bool isLoop)
{
	// ��Ԃ̉񐔎擾 (�_�~�[�_�������̂� -2�_)
	int32_t lerpCount = (int32_t)controllPoints.size() - 2;

	// �z������̂ł���ΏI�_�Ǝn�_���̕�Ԃ�ǉ�
	if (isLoop == true) 
	{
		lerpCount++;
	}

	// ��Ԗ��ɂ����鎞�Ԃ��ԉ񐔂ƍ��v���Ԃ���擾
	segmentTime = allTime / (float)lerpCount;

	// �n�_�I�_�̃R�s�[�Ń_�~�[���W��ݒ�
	dummyPoints[0] = controllPoints.front();
	dummyPoints[1] = controllPoints.back();

	// ���ݒn�_�̎擾
	currentPosition = controllPoints[0];

	// �z�����邩������������󂯎��
	this->isLoop = isLoop;

	// �^�C�}�[�N��,�C���f�b�N�X�����������Ă���
	startIndex = 0;
	tData.Start(segmentTime);
}

void SplineCurve::Update()
{
	// �^�C�}�[�f�[�^�̍X�V
	tData.Update();

	float timeRate = tData.GetTimeRate();
	// ��Ԃ��I�������
	if (timeRate >= 1.0f) 
	{
		// �ēx�^�C�}�[�Z�b�g�A�C���f�b�N�X��i�߂�
		tData.Start(segmentTime);
		startIndex++;
		timeRate = 0.0f;
	}
	
	// �⊮�p���W,�f�[�^
	Vector3 points[4] = {};
	int32_t index[4] = {};

	// �ő��Ԑ�
	size_t maxLerpNum = 0;
	if (isLoop) 
	{
		maxLerpNum = controllPoints.size() - 1;
	}
	else 
	{
		maxLerpNum = controllPoints.size() - 2;
	}

	// �z����̂ł����
	if (isLoop == true)
	{
		int32_t backIndex;
		backIndex = (int32_t)controllPoints.size() - 1;

		// �z�p�A�C���f�b�N�X���I�[�o�[�t���[�����Ƃ���0�ɖ߂�
		if (startIndex > maxLerpNum)
		{
			startIndex = 0;
		}

#pragma region �X�v���C�����
		if (startIndex == 0) 
		{
			index[0] = backIndex;
			index[1] = startIndex;
			index[2] = startIndex + 1;
			index[3] = startIndex + 2;
		}
		else if (startIndex == backIndex - 1) 
		{
			index[0] = startIndex - 1;
			index[1] = startIndex;
			index[2] = startIndex + 1;
			index[3] = 0;
		}
		else if (startIndex == backIndex) 
		{
			index[0] = startIndex - 1;
			index[1] = startIndex;
			index[2] = 0;
			index[3] = 1;
		}
		else 
		{
			index[0] = startIndex - 1;
			index[1] = startIndex;
			index[2] = startIndex + 1;
			index[3] = startIndex + 2;
		}

		points[0] = controllPoints[index[0]];
		points[1] = controllPoints[index[1]];
		points[2] = controllPoints[index[2]];
		points[3] = controllPoints[index[3]];

		Vector3 position = 0.5 *
		( 2 * points[1] + (-1 * points[0] + points[2]) * (timeRate)+
		( 2 * points[0] - 5 * points[1] + 4 * points[2] - points[3]) * (timeRate * timeRate) +
		(-1 * points[0] + 3 * points[1] - 3 * points[2] + points[3]) * (timeRate * timeRate * timeRate));

		currentPosition = position;
#pragma endregion
	}
	else // �z���Ȃ��̂ł����
	{
#pragma region �X�v���C�����
		if (startIndex == maxLerpNum)
		{
			points[3] = dummyPoints[1];
		}
		else if (startIndex > maxLerpNum)
		{
			currentPosition = dummyPoints[1];
			return;
		}
		else
		{
			points[3] = controllPoints[startIndex + 2];
		}

		if (startIndex == 0)
		{
			points[0] = dummyPoints[0];
		}
		else if (startIndex < 0)
		{
			currentPosition = dummyPoints[0];
			return;
		}
		else
		{
			points[0] = controllPoints[startIndex - 1];
		}
		points[1] = controllPoints[startIndex];
		points[2] = controllPoints[startIndex + 1];

		Vector3 position = 0.5 *
		( 2 * points[1] + (-1 * points[0] + points[2]) * (timeRate)+
		( 2 * points[0] - 5 * points[1] + 4 * points[2] - points[3]) * (timeRate * timeRate) +
		(-1 * points[0] + 3 * points[1] - 3 * points[2] + points[3]) * (timeRate * timeRate * timeRate));

		currentPosition = position;
#pragma endregion
	}
}