#pragma once
#include "Common.h"

class TimeData
{
private:
	// 最大時間
	float maxTime_ = 0;

	// 現在時間
	float nowTime_ = 0;

	// 0.f - 1.fに変換するデータ
	float timeRate_ = 0;

	// タイマーのアクティブ
	bool isActive_ = false;

public:
	/// <summary>
	/// タイマーの開始
	/// </summary>
	/// <param name="allFrame">全体フレーム数</param>
	void Start(float allFrames);

	// 更新処理
	void Update();

	// 現在時間のゲッター
	float GetTime() { return nowTime_; }

	// レートのゲッター
	float GetTimeRate() { return timeRate_; }

	// 1 - レートのゲッター
	float GetDisTimeRate() { return 1.f - timeRate_; }

	// アクティブのゲッター
	bool GetActive() { return isActive_; }
}; 