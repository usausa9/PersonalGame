#pragma once
#include "UsaMath.h"

#include <vector>

class SplineCurve
{
public:	// メンバ関数
	void Update();

	// 制御点セット
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }

private:
	// 制御点の配列
	std::vector<Vector3> controllPoints = {};
};

