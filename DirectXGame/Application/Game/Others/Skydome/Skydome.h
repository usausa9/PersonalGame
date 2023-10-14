#pragma once
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
};