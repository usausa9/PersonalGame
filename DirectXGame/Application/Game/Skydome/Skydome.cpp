#include "Skydome.h"

void Skydome::Initialize()
{
	// 自機モデル読み込み
	skydomeModel = OBJModel::LoadFromOBJ("skydome");

	// 自機の行列初期化
	skydomeObj.scale = { 1000,1000,1000 };
	skydomeObj.rotation = { 0, 0, 0 };
	skydomeObj.position = { 0, 0, 0 };
	skydomeObj.InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	skydomeObj.objModel = &skydomeModel;
}

void Skydome::Update()
{
	skydomeObj.UpdateObject3D();
}

void Skydome::Draw()
{
	// オブジェ描画
	skydomeObj.DrawObject3D();
}
