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

	camera = new Camera;
	camera->Initialize();

	// プレイヤー初期化
	player = make_unique<Player>();
	player.get()->Initialize();
	
	railCamera = new RailCamera();
	// レールカメラ初期化
	railCamera->Initialize({0, 0, -20.0f}, {0, 0, 0});
	
	// カメラ初期化
	//camera = railCamera->GetCamera();

	/*camera->target = { 0,0,0 };
	camera->position = { 0,0,-20.0f };*/
}

void GameScene::Finalize()
{
	//delete camera;
	delete railCamera;
}

void GameScene::Update()
{
	// DIrectX毎フレーム処理(更新処理) ここから
	railCamera->Update();

	player->SetParent(railCamera->GetObject3d());
	player->Update();

	//// アローキーでカメラ移動
	//if (Key::Down(DIK_LEFT))
	//{
	//	camera->position.x -= 0.5f;
	//}
	//else if (Key::Down(DIK_RIGHT))
	//{
	//	camera->position.x += 0.5f;
	//}

	//if (Key::Down(DIK_DOWN))
	//{
	//	camera->position.y -= 0.5f;
	//}
	//else if (Key::Down(DIK_UP))
	//{
	//	camera->position.y += 0.5f;
	//}
	
	camera = railCamera->GetCamera();
	camera->Update();
}

void GameScene::Draw3D()
{
	// カメラセット
	camera->Set();

	// 3Dオブジェ描画
	player.get()->Draw();
}

void GameScene::DrawParticle()
{
	// カメラセット
	camera->Set();

	// パーティクルオブジェ描画
}

void GameScene::Draw2D()
{
}