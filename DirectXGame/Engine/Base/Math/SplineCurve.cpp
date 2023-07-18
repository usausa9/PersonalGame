#include "SplineCurve.h"

void SplineCurve::MoveStart(float allTime, bool isLoop)
{
	// 補間の回数取得 (ダミー点を除くので -2点)
	int32_t lerpCount = (int32_t)controllPoints.size() - 2;

	// 循環させるのであれば終点と始点分の補間を追加
	if (isLoop == true) 
	{
		lerpCount++;
	}

	// 区間毎にかける時間を補間回数と合計時間から取得
	segmentTime = allTime / (float)lerpCount;

	// 始点終点のコピーでダミー座標を設定
	dummyPoints[0] = controllPoints.front();
	dummyPoints[1] = controllPoints.back();

	// 現在地点の取得
	currentPosition = controllPoints[0];

	// 循環させるかを仮引数から受け取り
	this->isLoop = isLoop;

	// タイマー起動,インデックスを初期化しておく
	startIndex = 0;
	tData.Start(segmentTime);
}

void SplineCurve::Update()
{
	// タイマーデータの更新
	tData.Update();

	float timeRate = tData.GetTimeRate();
	// 補間が終わったら
	if (timeRate >= 1.0f) 
	{
		// 再度タイマーセット、インデックスを進める
		tData.Start(segmentTime);
		startIndex++;
		timeRate = 0.0f;
	}
	
	// 補完用座標,データ
	Vector3 points[4] = {};
	int32_t index[4] = {};

	// 最大補間数
	size_t maxLerpNum = 0;
	if (isLoop) 
	{
		maxLerpNum = controllPoints.size() - 1;
	}
	else 
	{
		maxLerpNum = controllPoints.size() - 2;
	}

	// 循環するのであれば
	if (isLoop == true)
	{
		int32_t backIndex;
		backIndex = (int32_t)controllPoints.size() - 1;

		// 循環用、インデックスがオーバーフローしたときに0に戻す
		if (startIndex > maxLerpNum)
		{
			startIndex = 0;
		}

#pragma region スプライン補間
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
	else // 循環しないのであれば
	{
#pragma region スプライン補間
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