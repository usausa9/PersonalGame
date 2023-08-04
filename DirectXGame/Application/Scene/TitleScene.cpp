#include "TitleScene.h"
#include "CollisionManager.h"

#include "FBXObject3D.h"

#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
{
	// 当たり判定
	collisionManager = CollisionManager::GetInstance();

	// パーティクル用のパイプライン・Init
	ParticleManager::CreatePipeline();

	// カメラ初期化
	camera = new Camera;
	camera->Initialize();

	camera->target = { 0,0,0 };
	camera->position = { 0,0,-20.0f };

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

void TitleScene::Finalize()
{
	delete camera;
}

void TitleScene::Update()
{
	// DIrectX毎フレーム処理(更新処理) ここから

	// プレイヤーの更新
	player->Update();

	// エネミーの更新
	enemy->Update();

	// 天球の行列更新
	skydome->Update();

	// カメラの更新
	camera->Update();

	// 全ての衝突をチェック (更新の最後)
	collisionManager->CheckAllCollisions();
}

void TitleScene::Draw3D()
{
	// カメラセット
	camera->Set();

	// 天球描画
	skydome->Draw();

	// プレイヤー描画
	player->Draw();

	// 敵描画
	/*enemy->Draw();*/
}

void TitleScene::DrawParticle()
{
	// カメラセット
	camera->Set();

	// パーティクルオブジェ描画
}

void TitleScene::Draw2D()
{
	
}