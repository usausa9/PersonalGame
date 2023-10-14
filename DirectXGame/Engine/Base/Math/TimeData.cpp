/**
 * @file	TimeData.cpp
 * @brief	時間管理やアニメーション用
**/

#include "TimeData.h"

#include <cassert>
#include <algorithm>
#include <functional>

void TimeData::Start(float allFrames)
{
	// 仮引数から最大フレームのセット
	maxTime_ = allFrames;

	// 現在時間の初期化
	nowTime_ = 0.0f;

	// タイマー動作をtrueに
	isActive_ = true;
}

void TimeData::Update()
{
	// アクティブ中でなければ更新しない
	if (!isActive_)
	{
		return;
	}

	// 現在時間を加算
	nowTime_ += 1.0f;

	// 0.f - 1.fに変換
	timeRate_ = nowTime_ / maxTime_;

	// 1を越えるなら1に, タイマー停止
	timeRate_ = min(timeRate_, 1.f);
	if (timeRate_ == 1.f)
	{
		isActive_ = false;
	}
}