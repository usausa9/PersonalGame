/**
 * @file	GameScene.cpp
 * @brief	ゲームシーン全体のデータ保持や初期化/更新/描画を行うもの
**/

#include "GameScene.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "ImGuiManager.h"

#include <fstream>

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
	player_.get()->Initialize();

	// 天球初期化
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();

	// シーン遷移後の残り
	purpleGroundTex_ = TextureManager::Load(L"Resources/Sprites/purple_ground.png");
	purpleGroundSprite_[0] = make_unique<Sprite>(purpleGroundTex_);
	purpleGroundSprite_[1] = make_unique<Sprite>(purpleGroundTex_);
	purpleGroundSprite_[0]->position_ = BASE_POS_;
	purpleGroundSprite_[1]->position_ = BASE_POS_;
	nowLoadingTex_ = TextureManager::Load(L"Resources/Sprites/now_loading.png");
	nowLoadingSprite_ = make_unique<Sprite>(nowLoadingTex_);
	nowLoadingSprite_->position_ = BASE_POS_;

	// 敵データ読み込み
	LoadCsvFile();
}

void GameScene::Finalize()
{
	delete railCamera_;
}

void GameScene::Update()
{
	if (!isEndTransition)
	{
		if (!groundAnimeTimer_.GetActive())
		{
			groundAnimeTimer_.Start(MAX_GROUND_ANIME_TIME_);
			loadingAnimeTimer_.Start(MAX_LOADING_ANIME_TIME_);
		}

		purpleGroundSprite_[0]->position_.x = BASE_POS_.x - (TRANSITION_MOVE_POS_ * Easing::In(groundAnimeTimer_.GetTimeRate(), 2.2f));
		purpleGroundSprite_[1]->position_.x = BASE_POS_.x + (TRANSITION_MOVE_POS_ * Easing::In(groundAnimeTimer_.GetTimeRate(), 2.2f));
		nowLoadingSprite_->position_.y = BASE_POS_.y - ((TRANSITION_MOVE_POS_ / 2) * Easing::In(groundAnimeTimer_.GetTimeRate(), 2.2f));

		groundAnimeTimer_.Update();
		loadingAnimeTimer_.Update();

		if (!loadingAnimeTimer_.GetActive())
		{
			if (!groundAnimeTimer_.GetActive())
			{
				isEndTransition = true;
			}
		}

		purpleGroundSprite_[0]->Update();
		purpleGroundSprite_[1]->Update();
		nowLoadingSprite_->Update();
	}
	else
	{
		waitTimer_.Update();

		// 敵の更新
		UpdateEnemyData();
		
		

		// ESCでタイトルに戻る
		// シーン切り替え依頼
		if (Key::Trigger(DIK_ESCAPE))
		{
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}


		if (Key::Trigger(DIK_Y))
		{
			EnemySpawn(uint8_t(EnemyKinds::NORMAL), uint8_t(TrajectoryKinds::LEFT_2_RIGHT));
		}
		else if (Key::Trigger(DIK_U))
		{
			EnemySpawn(uint8_t(EnemyKinds::POWER), uint8_t(TrajectoryKinds::RIGHT_2_LEFT));
		}
	}
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

	// 天球の行列更新
	skydome_->Update();

	// カメラをレールカメラのものへ
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// 全ての衝突をチェック (更新の最後)
	collisionManager_ -> CheckAllCollisions();
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

	purpleGroundSprite_[0]->Draw();
	purpleGroundSprite_[1]->Draw();
	nowLoadingSprite_->Draw();
}

void GameScene::EnemySpawn(uint8_t enemyKind, uint8_t trajectoryKind)
{
	std::vector<Vector3> enemyMovePoints = TrajectoryKind(trajectoryKind);

	// 敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints, enemyKind);
	newEnemy->Spawn();
	// リストに登録
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::LoadCsvFile()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/Data/csv/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字ストリームにコピー
	enemyData_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

std::vector<Vector3> GameScene::TrajectoryKind(uint8_t trajectoryKind)
{
	if (trajectoryKind == uint8_t(TrajectoryKinds::LEFT_2_RIGHT))
	{
		// スプライン制御点
		float z = 40.0f;
		Vector3 start = { -20,  0, z };
		Vector3 p1 = { -10, 10, z };
		Vector3 p2 = { 0,  0, z };
		Vector3 p3 = { 10,-10, z };
		Vector3 end = { 20,  0, z };

		std::vector<Vector3> movePoints = { start, p1, p2, p3, end };

		return movePoints;
	}
	else if (trajectoryKind == uint8_t(TrajectoryKinds::RIGHT_2_LEFT))
	{
		// スプライン制御点
		float z = 40.0f;
		Vector3 start = { 20,  0, z };
		Vector3 p1 = { 10,-10, z };
		Vector3 p2 = { 0,  0, z };
		Vector3 p3 = { -10, 10, z };
		Vector3 end = { -20,  0, z };

		std::vector<Vector3> movePoints = { start, p1, p2, p3, end };

		return movePoints;
	}
	else
	{
		// スプライン制御点
		float z = 40.0f;
		Vector3 start = { 0, 0, z };
		Vector3 p1 = { 0, 0, z };
		Vector3 p2 = { 0, 0, z };
		Vector3 p3 = { 0, 0, z };
		Vector3 end = { 0, 0, z };

		std::vector<Vector3> movePoints = { start, p1, p2, p3, end };

		return movePoints;
	}
}

void GameScene::UpdateEnemyData()
{
	// 待機処理
	if (isStandBy_)
	{
		if (waitTimer_.GetActive() == false)
		{
			// 待機終了
			isStandBy_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyData_, line))
	{
		// 1行分の文字数をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// カンマ区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// コメントアウト
		if (word.find("//") == 0)
		{
			// 行を飛ばす
			continue;
		}

		// POP
		if (word.find("POP") == 0)
		{
			// 敵の種類
			getline(line_stream, word, ',');
			uint8_t kind = static_cast<uint8_t>(std::atoi(word.c_str()));

			// 敵の軌道
			getline(line_stream, word, ',');
			uint8_t trajectory = static_cast<uint8_t>(std::atoi(word.c_str()));

			EnemySpawn(kind, trajectory);
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// 待ち時間
			float waitTimeData = float(std::atoi(word.c_str()));

			// 待機開始
			isStandBy_ = true;
			waitTimer_.Start(waitTimeData);

			// コマンドループを抜ける
			break;
		}
	}
}