#include "GameScene.h"

#include "FBXObject3D.h"

using namespace Input;

void GameScene::Initialize()
{
	// デバイスをセット
	FBXObject3D::SetDevice(DirectXBase::Get()->device.Get());
	// カメラをセット
	FBXObject3D::SetCamera(camera);
	// コマンドリスト初期化
	FBXObject3D::SetCommandList(DirectXBase::Get()->commandList.Get());
	// グラフィックスパイプライン生成
	FBXObject3D::CreateGraphicsPipeline();
	
	// テクスチャ読み込み

	// スプライト・テクスチャ紐づけ

	// パーティクル用のパイプライン・Init
	ParticleManager::CreatePipeline();

	// OBJ読み込み

	// Object3D Init

	// objとObject3Dの紐付け

	// FBXモデル関連

	// カメラ初期化
	camera->Initialize();

	//camera->target = { 0,20,0 };
	//camera->position = { 0,0,-100 };
}

void GameScene::Finalize()
{

}

void GameScene::Update()
{

	if (Key::Down(DIK_A) || Key::Down(DIK_LEFT))
	{
		camera->position.x -= 0.5f;
	}
	else if (Key::Down(DIK_D) || Key::Down(DIK_RIGHT))
	{
		camera->position.x += 0.5f;
	}

	if (Key::Down(DIK_S) || Key::Down(DIK_DOWN))
	{
		camera->position.y -= 0.5f;
	}
	else if (Key::Down(DIK_W) || Key::Down(DIK_UP))
	{
		camera->position.y += 0.5f;
	}
	
	camera->Update();
}

void GameScene::Draw3D()
{
	// カメラセット
	camera->Set();

	// 3Dオブジェ描画

}

void GameScene::DrawParticle()
{
	// カメラセット
	camera->Set();

	// パーティクル描画
}

void GameScene::Draw2D()
{
	// 前景スプライト描画
}