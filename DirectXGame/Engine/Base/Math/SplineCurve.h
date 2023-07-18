#pragma once
#include "UsaMath.h"
#include "TimeData.h"

#include <vector>

class SplineCurve
{
private: // メンバ変数
	// 制御点の配列
	std::vector<Vector3> controllPoints = {};

	// 現在地点
	Vector3 currentPosition = {};
	// スプライン補間用の始点終点用の制御点2つ
	Vector3 dummyPoints[2] = {};

	// タイマーデータ
	TimeData tData = {};

	// 1区間あたりの補間時間
	float segmentTime = 0;

	// 補間開始点
	int32_t startIndex = 0;

	// 終点と始点を補間して循環させるかどうか
	bool isLoop = false;

public:	// メンバ関数
	/// <summary>
	/// スプライン補間用の関数
	/// </summary>
	/// <param name="allTime">補間の合計時間(循環させる場合は一周あたりの時間)</param>
	/// <param name="isLoop">循環させるかどうか</param>
	void MoveStart(float allTime, bool isLoop);

	// 更新処理
	void Update();

	// 制御点セット
	void SetPositions(std::vector<Vector3> points) { this->controllPoints = points; }
	
	// 現在位置取得
	Vector3 GetNowPosition() const { return currentPosition; }

};