#pragma once
#include "Common.h"

class TimeData
{
private:
	// 最大時間
	float maxTime = 0;

	// 現在時間
	float nowTime = 0;

	// 0.f - 1.fに変換するデータ
	float timeRate = 0;

	// タイマーのアクティブ
	bool isActive = false;

public:
	/// <summary>
	/// タイマーの開始
	/// </summary>
	/// <param name="allFrame">全体フレーム数</param>
	void Start(float allFrames);

	// 更新処理
	void Update();

	// レートのゲッター
	float GetTimeRate() { return timeRate; }
};

