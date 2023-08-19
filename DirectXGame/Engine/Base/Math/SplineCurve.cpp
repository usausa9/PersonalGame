#include "SplineCurve.h"

void SplineCurve::MoveStart(float allTime, bool isLoop)
{
	// 補間の回数取得 (ダミー点を除くので -2点)
	int32_t lerpCount = (int32_t)controllPoints_.size() - 2;

	// 循環させるのであれば終点と始点分の補間を追加
	if (isLoop == true) 
	{
		lerpCount++;
	}

	// 区間毎にかける時間を補間回数と合計時間から取得
	segmentTime_ = allTime / (float)lerpCount;

	// 始点終点のコピーでダミー座標を設定
	dummyPoints_[0] = controllPoints_.front();
	dummyPoints_[1] = controllPoints_.back();

	// 現在地点の取得
	currentPosition_ = controllPoints_[0];

	// 循環させるかを仮引数から受け取り
	this->isLoop_ = isLoop;

	// タイマー起動,インデックスを初期化しておく
	startIndex_ = 0;
	tData_.Start(segmentTime_);
}

void SplineCurve::Update()
{
	// タイマーデータの更新
	tData_.Update();

	float timeRate = tData_.GetTimeRate();
	// 補間が終わったら
	if (timeRate >= 1.0f) 
	{
		// 再度タイマーセット、インデックスを進める
		tData_.Start(segmentTime_);
		startIndex_++;
		timeRate = 0.0f;
	}
	
	// 補完用座標,データ
	Vector3 points[4] = {};
	int32_t index[4] = {};

	// 最大補間数
	size_t maxLerpNum = 0;
	if (isLoop_) 
	{
		maxLerpNum = controllPoints_.size() - 1;
	}
	else 
	{
		maxLerpNum = controllPoints_.size() - 2;
	}

	// 循環するのであれば
	if (isLoop_ == true)
	{
		int32_t backIndex;
		backIndex = (int32_t)controllPoints_.size() - 1;

		// 循環用、インデックスがオーバーフローしたときに0に戻す
		if (startIndex_ > maxLerpNum)
		{
			startIndex_ = 0;
		}

#pragma region スプライン補間
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
	else // 循環しないのであれば
	{
#pragma region スプライン補間
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