#include "GameScene.h"
#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void GameScene::Initialize()
{
	// 当たり判定
	collisionManager = CollisionManager::GetInstance();

	// パーティクル用のパイプライン・Init
	ParticleManager::CreatePipeline();

	// カメラ初期化
	camera = new Camera;
	camera->Initialize();

	// レールカメラ初期化
	railCamera = new RailCamera();
	railCamera->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// プレイヤー初期化
	player = make_unique<Player>();
	player.get()->Initialize(camera);

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
	for (std::unique_ptr<Enemy>& enemy : enemys) 
	{
		enemy->Update(railCamera->GetObject3d()->matWorld, player->GetWorldPosition());
	}

	// 死んでる敵を消す
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) 
	{
		if (!enemy->IsAlive()) 
		{
			return true;
		}

		return false; 
	});

	if (Key::Trigger(DIK_Q)) 
	{
		EnemySpawn();
	}

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
	for (std::unique_ptr<Enemy>& enemy : enemys) 
	{
		enemy->Draw();
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
	player->DrawUI();
}

void GameScene::EnemySpawn()
{
	float z = 40.0f;

	Vector3 start { -10,0,z };
	Vector3 p1 =  { 0,5,z };
	Vector3 p2 =  { -5,0,z };
	Vector3 p3 =  { 5,0,z };
	Vector3 end = { 10,0,z };

	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,end };

	// 敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->Spawn();

	// リストに登録
	enemys.push_back(std::move(newEnemy));
}