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
	vicviper = OBJModel::LoadFromOBJ("vicviper");
	ico = OBJModel::LoadFromOBJ("ICO");

	// Object3D Init
	playerObj.position = { 0, 0, 0 };
	playerObj.scale = { 1, 1, 1 };
	playerObj.rotation = { 20 * (UsaMath::u_PI / 180), 0, 0};
	playerObj.InitializeObject3D();

	playerBulletObj.position = { -100.0f, 0, 0 };
	playerBulletObj.InitializeObject3D();
	playerBulletObj.objModel = &ico;

	// objとObject3Dの紐付け
	player = make_unique<Player>();
	player.get()->Initialize();
	playerObj.objModel = &vicviper;

	// FBXモデル関連
	// モデル名を指定してFBXファイル読み込み

	// カメラ初期化
	camera->Initialize();

	camera->target = { 0,0,0 };
	camera->position = { 0,0,20.0f };
}

void GameScene::Finalize()
{
	delete camera;
}

void GameScene::Update()
{
	// DIrectX毎フレーム処理(更新処理) ここから

	player.get()->Update();

	playerObj.UpdateObject3D();
	playerBulletObj.UpdateObject3D();

	if (Key::Down(DIK_D) && Key::Down(DIK_A))
	{

	}
	else if (Key::Down(DIK_D) && playerObj.position.x >= -25.0f)
	{
		playerObj.position += { -velocity, 0, 0 };
	}
	else if (Key::Down(DIK_A) && playerObj.position.x <= 25.0f)
	{
		playerObj.position += { velocity, 0, 0 };
	}


	if (Key::Down(DIK_W) && Key::Down(DIK_S))
	{

	}
	else if (Key::Down(DIK_W) && playerObj.position.y <= 13.0f)
	{
		playerObj.position += { 0, velocity * 9 / 14, 0 };
	}
	else if (Key::Down(DIK_S) && playerObj.position.y >= -13.0f)
	{
		playerObj.position += { 0, -velocity * 9 / 14, 0 };
	}

	if (Key::Trigger(DIK_SPACE))
	{
		bulletLive = true;
		playerBulletObj.position = playerObj.position + Vector3{0, 0.6f, -5.9f};
	}

	if (bulletLive == true)
	{
		playerBulletObj.position -= { 0, 0, 1.3f };
	}

	if (playerBulletObj.position.z < -50.0f)
	{
		bulletLive = false;
	}

	if (Key::Down(DIK_LEFT))
	{
		camera->position.x -= 0.5f;
	}
	else if (Key::Down(DIK_RIGHT))
	{
		camera->position.x += 0.5f;
	}

	if (Key::Down(DIK_DOWN))
	{
		camera->position.y -= 0.5f;
	}
	else if (Key::Down(DIK_UP))
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
	//playerObj.DrawObject3D();
	player.get()->Draw();

	if (bulletLive == true)
	{
		playerBulletObj.DrawObject3D();
	}

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