/**
 * @file	Skydome.cpp
 * @brief	天球
**/

#include "Skydome.h"

void Skydome::Initialize()
{
	// 自機モデル読み込み
	skydomeModel_ = OBJModel::LoadFromOBJ("skydome");

	// 自機の行列初期化
	skydomeObj_.scale_ = { 1000,1000,1000 };
	skydomeObj_.rotation_ = { 0, 0, 0 };
	skydomeObj_.position_ = { 0, 0, 0 };
	skydomeObj_.InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	skydomeObj_.objModel_ = &skydomeModel_;
}

void Skydome::Update()
{
	skydomeObj_.UpdateObject3D();
}

void Skydome::Draw()
{
	// オブジェ描画
	skydomeObj_.DrawObject3D();
}
