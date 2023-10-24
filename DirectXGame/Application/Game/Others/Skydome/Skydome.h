#pragma once
/**
 * @file	Skydome.h
 * @brief	天球
**/

#include "Object3d.h"

class Skydome
{
public:
	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画
	void Draw();

private:
	// 天球モデル, 天球オブジェクト
	OBJModel skydomeModel_;
	Object3D skydomeObj_;

	const Vector3 SKYDOME_SCALE_ = { 1000, 1000, 1000} ;
};