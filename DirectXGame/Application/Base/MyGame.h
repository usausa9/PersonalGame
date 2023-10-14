#pragma once

/**
 * @file	MyGame.h
 * @brief	Scene処理の枠組み
**/

#include "UsaFramework.h"

class MyGame : public UsaFramework
{
public:	// メンバ関数
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;
};