#pragma once
#include "UsaMath.h"
#include "TimeData.h"

#include <vector>

class SplineCurve
{
private: // �����o�ϐ�
	// ����_�̔z��
	std::vector<Vector3> controllPoints_ = {};

	// ���ݒn�_
	Vector3 currentPosition_ = {};
	// �X�v���C����ԗp�̎n�_�I�_�p�̐���_2��
	Vector3 dummyPoints_[2] = {};

	// �^�C�}�[�f�[�^
	TimeData tData_ = {};

	// 1��Ԃ�����̕�Ԏ���
	float segmentTime_ = 0;

	// ��ԊJ�n�_
	int16_t startIndex_ = 0;

	// �I�_�Ǝn�_���Ԃ��ďz�����邩�ǂ���
	bool isLoop_ = false;

public:	// �����o�֐�
	/// <summary>
	/// �X�v���C����ԗp�̊֐�
	/// </summary>
	/// <param name="allTime">��Ԃ̍��v����(�z������ꍇ�͈��������̎���)</param>
	/// <param name="isLoop">�z�����邩�ǂ���</param>
	void MoveStart(float allTime, bool isLoop);

	// �X�V����
	void Update();

	// ����_�Z�b�g
	void SetPositions(std::vector<Vector3> points) { this->controllPoints_ = points; }
	
	// ���݈ʒu�擾
	Vector3 GetNowPosition() const { return currentPosition_; }
};