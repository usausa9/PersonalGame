#pragma once
#include "UsaMath.h"

#include <vector>

class SplineCurve
{
public:	// �����o�֐�
	void Update();

	// ����_�Z�b�g
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

private:
	// ����_�̔z��
	std::vector<Vector3> controllPoints = {};
};

