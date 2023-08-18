#include "GameScene.h"
#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void GameScene::Initialize()
{
	// 当たり判定
	collisionManager_ = CollisionManager::GetInstance();

	// パーティクル用のパイプライン・Init
	ParticleManager::CreatePipeline();

	// カメラ初期化
	camera_ = new Camera;
	camera_->Initialize();

	// レールカメラ初期化
	railCamera_ = new RailCamera();
	railCamera_->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// プレイヤー初期化
	player_ = make_unique<Player>();
	player_.get()->Initialize(camera_);

	// 天球初期化
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();
}

void GameScene::Finalize()
{
	delete railCamera_;
}

void GameScene::Update()
{
	// DirectX毎フレーム処理(更新処理) ここから
	railCamera_->Update();

	// プレイヤーの更新
	player_->SetParent(railCamera_->GetObject3d());
	player_->Update();

	// エネミーの更新
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update(railCamera_->GetObject3d()->matWorld_);
	}

	// 死んでる敵を消す
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)
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
	skydome_->Update();
	
	// カメラをレールカメラのものへ
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// 全ての衝突をチェック (更新の最後)
	collisionManager_->CheckAllCollisions();
}

void GameScene::Draw3D()
{
	// カメラセット
	camera_->Set();

	// 天球描画
	skydome_->Draw();

	// プレイヤー描画
	player_->Draw();

	// 敵描画
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void GameScene::DrawParticle()
{
	// カメラセット
	camera_->Set();

	// パーティクルオブジェ描画
}

void GameScene::Draw2D()
{
	player_->DrawUI();
}

void GameScene::EnemySpawn()
{
	// スプライン制御点
	float z = 40.0f;
	Vector3 start { 0,0,z };
	Vector3 p1 =  { 5,5,z };
	Vector3 p2 =  { -5,-5,z };
	Vector3 p3 =  { 5,5,z };
	Vector3 end = { -10,-10,z };

	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,end };

	// 敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->Spawn();

	// リストに登録
	enemys_.push_back(std::move(newEnemy));
}