#include "TimeData.h"

#include <cassert>
#include <algorithm>
#include <functional>

void TimeData::Start(float allFrames)
{
	// 仮引数から最大フレームのセット
	maxTime = allFrames;

	// 現在時間の初期化
	nowTime = 0.0f;

	// タイマー動作をtrueに
	isActive = true;
}

void TimeData::Update()
{
	// アクティブ中でなければ更新しない
	if (!isActive) 
	{
		return;
	}

	// 現在時間を加算
	nowTime += 1.0f;

	// 0.f - 1.fに変換
	timeRate = nowTime / maxTime;

	// 1を越えるなら1に, タイマー停止
	timeRate = min(timeRate, 1.f);
	if (timeRate == 1.f) 
	{
		isActive = false;
	}
}