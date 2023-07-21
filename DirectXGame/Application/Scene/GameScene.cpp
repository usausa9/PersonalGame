#include "GameScene.h"
#include "CollisionManager.h"

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

	// 当たり判定
	collisionManager = CollisionManager::GetInstance();

	
	
	// テクスチャ読み込み

	// スプライト・テクスチャ紐づけ

	// パーティクル用のパイプライン・Init
	ParticleManager::CreatePipeline();

	// OBJ読み込み

	// Object3D Init

	// objとObject3Dの紐付け

	// カメラ初期化
	camera = new Camera;
	camera->Initialize();

	// レールカメラ初期化
	railCamera = new RailCamera();
	railCamera->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// プレイヤー初期化
	player = make_unique<Player>();
	player.get()->Initialize(camera);
	
	// エネミー初期化
	enemy = make_unique<Enemy>();
	enemy.get()->Initialize();

	// 天球初期化
	skydome = make_unique<Skydome>();
	skydome.get()->Initialize();
}

void GameScene::Finalize()
{
	delete railCamera;
}

void GameScene::Update()
{
	// DIrectX毎フレーム処理(更新処理) ここから
	railCamera->Update();

	// プレイヤーの更新
	player->SetParent(railCamera->GetObject3d());
	player->Update();

	// エネミーの更新
	enemy->Update();

	// 天球の行列更新
	skydome->Update();
	
	// カメラをレールカメラのものへ
	camera = railCamera->GetCamera();
	camera->Update();

	// 全ての衝突をチェック (更新の最後)
	collisionManager->CheckAllCollisions();
}

void GameScene::Draw3D()
{
	// カメラセット
	camera->Set();

	// 天球描画
	skydome->Draw();

	// プレイヤー描画
	player->Draw();

	// 敵描画
	enemy->Draw();
}

void GameScene::DrawParticle()
{
	// カメラセット
	camera->Set();

	// パーティクルオブジェ描画
}

void GameScene::Draw2D()
{
	player->DrawUI();
}