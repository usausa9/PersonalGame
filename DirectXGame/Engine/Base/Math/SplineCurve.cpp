#include "SplineCurve.h"

void SplineCurve::MoveStart(float allTime, bool isLoop)
{
	// ��Ԃ̉񐔎擾 (�_�~�[�_�������̂� -2�_)
	int32_t lerpCount = (int32_t)controllPoints_.size() - 2;

	// �z������̂ł���ΏI�_�Ǝn�_���̕�Ԃ�ǉ�
	if (isLoop == true) 
	{
		lerpCount++;
	}

	// ��Ԗ��ɂ����鎞�Ԃ��ԉ񐔂ƍ��v���Ԃ���擾
	segmentTime_ = allTime / (float)lerpCount;

	// �n�_�I�_�̃R�s�[�Ń_�~�[���W��ݒ�
	dummyPoints_[0] = controllPoints_.front();
	dummyPoints_[1] = controllPoints_.back();

	// ���ݒn�_�̎擾
	currentPosition_ = controllPoints_[0];

	// �z�����邩������������󂯎��
	this->isLoop_ = isLoop;

	// �^�C�}�[�N��,�C���f�b�N�X�����������Ă���
	startIndex_ = 0;
	tData_.Start(segmentTime_);
}

void SplineCurve::Update()
{
	// �^�C�}�[�f�[�^�̍X�V
	tData_.Update();

	float timeRate = tData_.GetTimeRate();
	// ��Ԃ��I�������
	if (timeRate >= 1.0f) 
	{
		// �ēx�^�C�}�[�Z�b�g�A�C���f�b�N�X��i�߂�
		tData_.Start(segmentTime_);
		startIndex_++;
		timeRate = 0.0f;
	}
	
	// �⊮�p���W,�f�[�^
	Vector3 points[4] = {};
	int32_t index[4] = {};

	// �ő��Ԑ�
	size_t maxLerpNum = 0;
	if (isLoop_) 
	{
		maxLerpNum = controllPoints_.size() - 1;
	}
	else 
	{
		maxLerpNum = controllPoints_.size() - 2;
	}

	// �z����̂ł����
	if (isLoop_ == true)
	{
		int32_t backIndex;
		backIndex = (int32_t)controllPoints_.size() - 1;

		// �z�p�A�C���f�b�N�X���I�[�o�[�t���[�����Ƃ���0�ɖ߂�
		if (startIndex_ > maxLerpNum)
		{
			startIndex_ = 0;
		}

#pragma region �X�v���C�����
		if (startIndex_ == 0) 
		{
			index[0] = backIndex;
			index[1] = startIndex_;
			index[2] = startIndex_ + 1;
			index[3] = startIndex_ + 2;
		}
		else if (startIndex_ == backIndex - 1) 
		{
			index[0] = startIndex_ - 1;
			index[1] = startIndex_;
			index[2] = startIndex_ + 1;
			index[3] = 0;
		}
		else if (startIndex_ == backIndex) 
		{
			index[0] = startIndex_ - 1;
			index[1] = startIndex_;
			index[2] = 0;
			index[3] = 1;
		}
		else 
		{
			index[0] = startIndex_ - 1;
			index[1] = startIndex_;
			index[2] = startIndex_ + 1;
			index[3] = startIndex_ + 2;
		}

		points[0] = controllPoints_[index[0]];
		points[1] = controllPoints_[index[1]];
		points[2] = controllPoints_[index[2]];
		points[3] = controllPoints_[index[3]];

		Vector3 position = 0.5 *
		( 2 * points[1] + (-1 * points[0] + points[2]) * (timeRate)+
		( 2 * points[0] - 5 * points[1] + 4 * points[2] - points[3]) * (timeRate * timeRate) +
		(-1 * points[0] + 3 * points[1] - 3 * points[2] + points[3]) * (timeRate * timeRate * timeRate));

		currentPosition_ = position;
#pragma endregion
	}
	else // �z���Ȃ��̂ł����
	{
#pragma region �X�v���C�����
		if (startIndex_ == maxLerpNum)
		{
			points[3] = dummyPoints_[1];
		}
		else if (startIndex_ > maxLerpNum)
		{
			currentPosition_ = dummyPoints_[1];
			return;
		}
		else
		{
			points[3] = controllPoints_[startIndex_ + 2];
		}

		if (startIndex_ == 0)
		{
			points[0] = dummyPoints_[0];
		}
		else if (startIndex_ < 0)
		{
			currentPosition_ = dummyPoints_[0];
			return;
		}
		else
		{
			points[0] = controllPoints_[startIndex_ - 1];
		}
		points[1] = controllPoints_[startIndex_];
		points[2] = controllPoints_[startIndex_ + 1];

		Vector3 position = 0.5 *
		( 2 * points[1] + (-1 * points[0] + points[2]) * (timeRate)+
		( 2 * points[0] - 5 * points[1] + 4 * points[2] - points[3]) * (timeRate * timeRate) +
		(-1 * points[0] + 3 * points[1] - 3 * points[2] + points[3]) * (timeRate * timeRate * timeRate));

		currentPosition_ = position;
#pragma endregion
	}
}