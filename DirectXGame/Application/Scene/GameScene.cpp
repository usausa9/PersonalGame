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
	railCamera_->Initialize(INIT_CAMERA_POSITION_, INIT_CAMERA_ROTATION_);

	// プレイヤー初期化
	player_ = make_unique<Player>();
	player_->Initialize();

	// ボス初期化
	boss_ = make_unique<Boss>();
	boss_->Initialize();

	// 天球初期化
	skydome_ = make_unique<Skydome>();
	skydome_->Initialize();

	enemyModel_[0] = OBJModel::LoadFromOBJ("Cube"); 
	enemyModel_[1] = OBJModel::LoadFromOBJ("ICO");

	// シーン遷移後の残り
	purpleGroundTex_ = TextureManager::Load(L"Resources/Sprites/purple_ground.png");

	for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
	{
		purpleGroundSprite_[i] = make_unique<Sprite>(purpleGroundTex_);
		purpleGroundSprite_[i]->position_ = GROUND_BASE_POS_;
	}
	
	nowLoadingTex_ = TextureManager::Load(L"Resources/Sprites/now_loading.png");
	nowLoadingSprite_ = make_unique<Sprite>(nowLoadingTex_);
	nowLoadingSprite_->position_ = GROUND_BASE_POS_;

	reticleTex_ = TextureManager::Load(L"Resources/Sprites/reticle.png");
	for (uint8_t i = 0; i < RETICLE_NUM_; i++)
	{
		reticleSprite_[i] = make_unique<Sprite>(reticleTex_);
		reticleSprite_[i]->position_ = RETICLE_BEFORE_ANIME_POS_[i];
		reticleSprite_[i]->scale_ = RETICLE_SIZE_;
		reticleSprite_[i]->Update();
	}

	clearTex_ = TextureManager::Load(L"Resources/Sprites/game_clear.png");
	clearSprite_ = make_unique<Sprite>(clearTex_);
	clearSprite_->position_ = CLEAR_UI_BASE_POS_;

	// 変数初期化
	playerStateUiPosY_ = PLAYER_STATE_UI_BEFORE_POS_Y_;
	isEndTransition_ = false;
	isEndStartAnimation_ = false;
	isUiAnimation_ = false;
	isGameOverAnimation_ = false;
	isGameClearAnimation_ = false;
	isGameOver_ = false;
	isGameClear_ = false; 
	isGameTimer_ = false;

	// ゲームオーバー演出スプライン制御点
	Vector3 dStart = DEAD_START_;
	Vector3 dP1 = DEAD_P1_;
	Vector3 dP2 = DEAD_P2_;
	Vector3 dP3 = DEAD_P3_;
	Vector3 dEnd = DEAD_END_;

	// クリア演出スプライン制御点
	Vector3 cStart = CLEAR_START_;
	Vector3 cP1 = CLEAR_P1_;
	Vector3 cP2 = CLEAR_P2_;
	Vector3 cP3 = CLEAR_P3_;
	Vector3 cEnd = CLEAR_END_;

	std::vector<Vector3> deadMovePoints = { dStart, dP1, dP2, dP3, dEnd };
	std::vector<Vector3> clearMovePoints = { cStart, cP1, cP2, cP3, cEnd };

	deadTrajectory_.SetPositions(deadMovePoints);
	clearTrajectory_.SetPositions(clearMovePoints);
	
	// 敵データ読み込み
	LoadCsvFile();
}

void GameScene::Finalize()
{
	delete railCamera_;
}

void GameScene::Update()
{
	// トランジション
	InTransition();

	// START演出
	BeforeStartAnimation();

	if (isGameTimer_ == false)
	{
		gameTimer_.Start(GAME_TIME_);
		isGameTimer_ = true;
	}

	if (gameTimer_.GetTimeRate() == BOSS_SPAWN_RATE_)
	{
		boss_.get()->Spawn();
	}
	
	gameTimer_.Update();

	if (isEndTransition_ && isEndStartAnimation_)
	{
		waitTimer_.Update();

		// ゲーム中のみ
		if (InGame())
		{
			// 敵の更新
			UpdateEnemyData();
		}

		// ESCでタイトルに戻る
		// シーン切り替え依頼
		if (Key::Trigger(DIK_ESCAPE) || Pad::Trigger(Button::START))
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

	// ゲーム中のみ
	if (InGame())
	{
		// DirectX毎フレーム処理(更新処理) ここから
		railCamera_->Update();
	}

	// ゲームオーバーの時の処理
	GameEnd();

	if (isEndTransition_ && isEndStartAnimation_)
	{
		// ゲームオーバーフラグ
		if (Key::Trigger(DIK_E) && isGameOverAnimation_ == false && isGameClear_ == false)
		{
			playerDeadPoint_ = player_->position_;
			isGameOver_ = true;
			isGameOverAnimation_ = true;

			deadTrajectory_.MoveStart(PLAYER_DEAD_MOVE_TIME_, false);

			for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
			{
				purpleGroundSprite_[i]->position_ = TRANSITION_BASE_POS_[i];
			}

			gameOverTimer_.Start(TRANSITION_WAIT_TIMER_);
		}
		// ゲームクリアフラグ
		if (Key::Trigger(DIK_R) && isGameClearAnimation_ == false && isGameOver_ == false)
		{
			playerClearPoint_ = player_->position_;
			isGameClear_ = true;
			isGameClearAnimation_ = true;

			clearTrajectory_.MoveStart(PLAYER_CLEAR_MOVE_TIME_, false);

			for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
			{
				purpleGroundSprite_[i]->position_ = TRANSITION_BASE_POS_[i];
			}

			gameClearTimer_.Start(TRANSITION_WAIT_TIMER_);
		}

		if (boss_.get()->IsAlive() == false && isGameClearAnimation_ == false && isGameOver_ == false)
		{
			playerClearPoint_ = player_->position_;
			isGameClear_ = true;
			isGameClearAnimation_ = true;

			clearTrajectory_.MoveStart(PLAYER_CLEAR_MOVE_TIME_, false);

			for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
			{
				purpleGroundSprite_[i]->position_ = TRANSITION_BASE_POS_[i];
			}

			gameClearTimer_.Start(TRANSITION_WAIT_TIMER_);
		}

		if (isGameOver_ || isGameClear_)
		{
			player_->Update(false, true, STANDARD_PLATER_POS_);
		}
		else
		{
			player_->Update(isEndStartAnimation_, true, STANDARD_PLATER_POS_);
		}
	}
	else
	{
		for (uint8_t i = 0; i < RETICLE_NUM_; i++)
		{
			reticleSprite_[i]->Update();
		}
		player_->Update(isEndStartAnimation_, false, inGameReticlePos_);
	}

	boss_->Update();

	// プレイヤーの更新
	player_->SetParent(railCamera_->GetObject3d());
	boss_->SetParent(railCamera_->GetObject3d());
	
	// ゲーム中のみ
	if (InGame())
	{
		// エネミーの更新
		for (std::unique_ptr<Enemy>& enemy : enemys_)
		{
			enemy->Update(railCamera_->GetObject3d()->matWorld_);
		}
	}

	clearSprite_->Update();
	
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

void GameScene::InTransition()
{
	if (!isEndTransition_)
	{
		if (!groundAnimeTimer_.GetActive())
		{
			groundAnimeTimer_.Start(GROUND_ANIME_MAX_TIME_);
			loadingAnimeTimer_.Start(LOADING_ANIME_MAX_TIME_);
		}

		purpleGroundSprite_[LEFT]->position_.x = GROUND_BASE_POS_.x - (TRANSITION_MOVE_POS_ * Easing::In(groundAnimeTimer_.GetTimeRate(), kEASING_TRANSITON_));
		purpleGroundSprite_[RIGHT]->position_.x = GROUND_BASE_POS_.x + (TRANSITION_MOVE_POS_ * Easing::In(groundAnimeTimer_.GetTimeRate(), kEASING_TRANSITON_));
		nowLoadingSprite_->position_.y = GROUND_BASE_POS_.y - ((TRANSITION_MOVE_POS_ / PURPLE_BG_NUM_) * Easing::In(groundAnimeTimer_.GetTimeRate(), kEASING_TRANSITON_));

		// アニメーション用タイマーの更新
		groundAnimeTimer_.Update();
		loadingAnimeTimer_.Update();

		if (!loadingAnimeTimer_.GetActive())
		{
			if (!groundAnimeTimer_.GetActive())
			{
				isEndTransition_ = true;
				playerStartAnimeTimer_.Start(PLAYER_START_ANIME_MAX_TIME_);
			}
		}
		
		for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
		{
			purpleGroundSprite_[i]->Update();
		}
		
		nowLoadingSprite_->Update();
	}
}

void GameScene::BeforeStartAnimation()
{
	if (!isEndStartAnimation_ && isEndTransition_)
	{
		// アニメーション用タイマーの更新
		playerStartAnimeTimer_.Update();
		uiAppearAnimeTimer_.Update();

		player_->position_ =
			PLAYER_BEFORE_ANIME_POS_ + PLAYER_ANIME_MOVE_ * Easing::Out(playerStartAnimeTimer_.GetTimeRate());

		if (!playerStartAnimeTimer_.GetActive() && isUiAnimation_ == false)
		{
			isUiAnimation_ = true;
			uiAppearAnimeTimer_.Start(UI_APPEAR_ANIME_MAX_TIME_);
		}

		if (!playerStartAnimeTimer_.GetActive() && !uiAppearAnimeTimer_.GetActive() && isUiAnimation_ == true)
		{
			isEndStartAnimation_ = true;
		}

		for (uint8_t i = 0; i < RETICLE_NUM_; i++)
		{
			reticlePos_[i] =
				RETICLE_BEFORE_ANIME_POS_[i] + RETICLE_ANIME_MOVE_[i] * Easing::Out(uiAppearAnimeTimer_.GetTimeRate());
			reticleSprite_[i]->position_ = reticlePos_[i];
		}

		inGameReticlePos_.y = 
			INGAME_RETICLE_BEFORE_ANIME_POS_Y_ + INGAME_RETICLE_ANIME_MOVE_Y_ * Easing::Out(uiAppearAnimeTimer_.GetTimeRate() , kEASING_UI_APPEAR_);

		playerStateUiPosY_ =
			PLAYER_STATE_UI_BEFORE_POS_Y_ - PLAYER_STATE_UI_MOVE_Y_ * Easing::Out(uiAppearAnimeTimer_.GetTimeRate());
	}
}

void GameScene::GameEnd()
{
	// ゲームオーバー演出
	if (isGameOver_)
	{
		deadTrajectory_.Update();
		gameOverTimer_.Update();
		gameOverTransitionTimer_.Update();

		if (gameOverTimer_.GetTimeRate() == TRANSITION_START_RATE_)
		{
			gameOverTransitionTimer_.Start(GROUND_ANIME_MAX_TIME_);
		}

		player_->GameOver();

		purpleGroundSprite_[LEFT]->position_.x = TRANSITION_BASE_POS_[LEFT].x + (TRANSITION_MOVE_POS_ * Easing::Out(gameOverTransitionTimer_.GetTimeRate(), kEASING_TRANSITON_));
		purpleGroundSprite_[RIGHT]->position_.x = TRANSITION_BASE_POS_[RIGHT].x - (TRANSITION_MOVE_POS_ * Easing::Out(gameOverTransitionTimer_.GetTimeRate(), kEASING_TRANSITON_));

		player_->position_ = playerDeadPoint_ + deadTrajectory_.GetNowPosition();
	}
	
	if (isGameClear_)
	{
		clearTrajectory_.Update();
		gameClearTimer_.Update();
		gameClearTransitionTimer_.Update();
		gameClearUiAnimeTimer_.Update();

		if (gameClearTimer_.GetTimeRate() == TRANSITION_START_RATE_)
		{
			gameClearTransitionTimer_.Start(GROUND_ANIME_MAX_TIME_);
		}

		player_->GameOver();

		purpleGroundSprite_[LEFT]->position_.x = TRANSITION_BASE_POS_[LEFT].x + (TRANSITION_MOVE_POS_ * Easing::Out(gameClearTransitionTimer_.GetTimeRate(), kEASING_TRANSITON_));
		purpleGroundSprite_[RIGHT]->position_.x = TRANSITION_BASE_POS_[RIGHT].x - (TRANSITION_MOVE_POS_ * Easing::Out(gameClearTransitionTimer_.GetTimeRate(), kEASING_TRANSITON_));

		player_->position_ = playerClearPoint_ + clearTrajectory_.GetNowPosition();
	
		if (gameClearTransitionTimer_.GetTimeRate() == CLEAR_START_RATE_)
		{
			gameClearUiAnimeTimer_.Start(CLEAR_UI_ANIME_MAX_TIME_);
		}

		clearSprite_->position_.y =
		CLEAR_UI_BEFORE_ANIME_POS_Y_ + CLEAR_UI_ANIME_MOVE_Y_ * Easing::In(gameClearUiAnimeTimer_.GetTimeRate());
	}

	for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
	{
		purpleGroundSprite_[i]->Update();
	}
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
	boss_->Draw();
}

void GameScene::DrawParticle()
{
	// カメラセット
	camera_->Set();

	// パーティクルオブジェ描画
}

void GameScene::Draw2D()
{
	player_->DrawUI(playerStateUiPosY_);

	for (uint8_t i = 0; i < PURPLE_BG_NUM_; i++)
	{
		purpleGroundSprite_[i]->Draw();
	}

	if (!isEndStartAnimation_)
	{
		for (uint8_t i = 0; i < RETICLE_NUM_; i++)
		{
			reticleSprite_[i]->Draw();
		}
	}

	clearSprite_->Draw();
	
	nowLoadingSprite_->Draw();
}

bool GameScene::InGame()
{
	if (isGameOver_ || isGameClear_)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void GameScene::EnemySpawn(uint8_t enemyKind, uint8_t trajectoryKind)
{
	// 軌道の設定
	std::vector<Vector3> enemyMovePoints = TrajectoryKind(trajectoryKind);

	// 敵の生成と初期化
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	if (enemyKind == uint8_t(EnemyKinds::NORMAL))
	{
		newEnemy->Initialize(enemyMovePoints, enemyKind, enemyModel_[0]);
	}
	if (enemyKind == uint8_t(EnemyKinds::POWER))
	{
		newEnemy->Initialize(enemyMovePoints, enemyKind, enemyModel_[1]);
	}
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