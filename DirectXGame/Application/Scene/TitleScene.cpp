/**
 * @file	TitleScene.cpp
 * @brief	タイトルシーン全体のデータ保持や初期化/更新/描画を行うもの
**/

#include "TitleScene.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
{
	// パーティクル用のパイプライン・Init
	ParticleManager::CreatePipeline();

	// カメラ初期化
	camera_ = new Camera;
	camera_->Initialize();

	// レールカメラ初期化
	railCamera_ = new RailCamera();
	railCamera_->Initialize(CAMERA_INIT_POS_, CAMERA_INIT_ROT_);

	// 天球
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();

	// 天球の行列更新
	skydome_->Update();

	// プレイヤー初期化
	player_ = make_unique<Player>();
	player_.get()->Initialize();
	player_->position_ = { PLAYER_HOME_POSITION_ };

	purpleGroundTex_ = TextureManager::Load(L"Resources/Sprites/purple_ground.png");
	purpleGroundSprite_ = make_unique<Sprite>(purpleGroundTex_);
	purpleGroundSprite_->position_ = TITLE_BASE_POS_;

	nowLoadingTex_ = TextureManager::Load(L"Resources/Sprites/now_loading.png");
	nowLoadingSprite_ = make_unique<Sprite>(nowLoadingTex_);
	nowLoadingSprite_->position_ = TITLE_BASE_POS_;
	
	titleSceneTex_ = TextureManager::Load(L"Resources/Sprites/title_scene.png");
	titleSceneSprite_ = make_unique<Sprite>(titleSceneTex_);
	titleSceneSprite_->position_ = TITLE_SCENE_BASE_POS_;

	pressKeyTex_ = TextureManager::Load(L"Resources/Sprites/press_key.png");
	pressKeySprite_ = make_unique<Sprite>(pressKeyTex_);
	pressKeySprite_->position_ = PRESS_KEY_BASE_POS_;

	purpleCircleTex_ = TextureManager::Load(L"Resources/Sprites/purple_circle.png");

	for (uint8_t i = 0; i < TRANSITION_CIRCLE_NUM_; i++)
	{
		purpleCircleSprite_[i] = make_unique<Sprite>(purpleCircleTex_);
		purpleCircleSprite_[i]->scale_ = TRANSITION_CIRCLE_INIT_SIZE_;
	}
	for (uint8_t i = 0; i < TRANSITION_CIRCLE_XY_NUM_.x; i++)
	{
		for (uint8_t j = 0; j < TRANSITION_CIRCLE_XY_NUM_.y; j++)
		{
			purpleCircleSprite_[spriteCount_]->position_ = {
			TRANSITION_CIRCLE_BASE_POS_.x + TRANSITION_CIRCLE_BASE_SIZE_.x * i,
			TRANSITION_CIRCLE_BASE_POS_.y + TRANSITION_CIRCLE_BASE_SIZE_.y * j, };
			spriteCount_++;
		}
	}

	// 変数初期化
	kPressKeySpriteSize_ = kPRESS_KEY_SPRITE_SIZE_;

	isBeforeTransition_ = false;
	isTransition_ = false;
	endAnimation_ = false;

	isTransHomePos_ = false;
	isWait_ = false;
	isDeparture = false;

	isDrawLoading_ = false;
	isNextCircleChange_ = true;

	transitionCircleScale_ = INIT_TRANSITION_CIRCLE_SCALE_;
	nowActiveTimer_ = INIT_TIMER_COUNT_;
	spriteCount_ = INIT_SPRITE_NUM_;
	needRotation_ = ROT_INIT_;
	reciprocalNeedRotation_ = ROT_INIT_;
}

void TitleScene::Finalize()
{
	delete railCamera_;
}

void TitleScene::Update()
{
	// DirectX毎フレーム処理(更新処理) ここから
	railCamera_->Update();

	// スタートフラグが立ったらゲームシーンへ
	PossibleStartGame();

	// シーン遷移関連
	if (isBeforeTransition_ == true)
	{
		BeforeTransition();
		player2TransHomePositionTimer_.Update();
		waitTimer_.Update();
	}

	if (isTransition_ == true)
	{
		Transition();
	}

	// タイマーの更新
	circleScaleChangeAllTimer_.Update();

	for (uint8_t i = 0; i < TRANSITION_CIRCLE_XY_NUM_.x; i++)
	{
		circleScaleChangeTimer_[i].Update();
	}

	// プレイヤーの更新
	if (!isTransition_)
	{
		if (playerRotateTimer_.GetActive() == false && isBeforeTransition_ == false)
		{
			playerRotateTimer_.Start(PLAYER_ROTATE_ANIME_MAX_TIME_);
		}

		player_->SetParent(railCamera_->GetObject3d());
		player_->Update(false, true, STANDARD_PLATER_POS_);
	}

	// カメラをレールカメラのものへ
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// pressKEYのアニメーションタイマー更新
	if (nowActiveTimer_ == EXPAND && !pressAnimeTimer_[SHRINK].GetActive())
	{
		pressAnimeTimer_[EXPAND].Start(PRESS_ANIME_MAX_TIME_);
		nowActiveTimer_ = SHRINK;
	}
	else if (nowActiveTimer_ == SHRINK && !pressAnimeTimer_[EXPAND].GetActive())
	{
		pressAnimeTimer_[SHRINK].Start(PRESS_ANIME_MAX_TIME_);
		nowActiveTimer_ = EXPAND;
	}

	if (!isTransition_)
	{
		if (isBeforeTransition_ == false)
		{
			player_->rotation_ = { UsaMath::DegreesToRadians(PLAYER_HOME_ROTATION_X_),
				 UsaMath::DegreesToRadians(playerRotateTimer_.GetTime()), 0 };

			playerRotateTimer_.Update();

			for (uint8_t i = 0; i < PRESS_ANIME_TIMER_NUM_; i++)
			{
				pressAnimeTimer_[i].Update();
			}
		}
	}

	if (nowActiveTimer_ == SHRINK)
	{
		kPressKeySpriteSize_ = {
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[EXPAND].GetTimeRate()) / kEASING_PLESS_SCALE_),
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[EXPAND].GetTimeRate()) / kEASING_PLESS_SCALE_) };
	}
	else
	{
		kPressKeySpriteSize_ = {
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[SHRINK].GetDisTimeRate()) / kEASING_PLESS_SCALE_),
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[SHRINK].GetDisTimeRate()) / kEASING_PLESS_SCALE_) };
	}

	pressKeySprite_->scale_ = { kPressKeySpriteSize_ };

	// タイトルスプライト更新
	titleSceneSprite_->Update();
	pressKeySprite_->Update();
	for (uint8_t i = 0; i < TRANSITION_CIRCLE_NUM_; i++)
	{
		purpleCircleSprite_[i]->Update();
	}

	purpleGroundSprite_->Update();
	nowLoadingSprite_->Update();
}

void TitleScene::Draw3D()
{
	// カメラセット
	camera_->Set();

	// 天球描画
	skydome_->Draw();

	if (!isTransition_)
	{
		// プレイヤー描画
		player_->Draw();
	}
}

void TitleScene::DrawParticle()
{
	// カメラセット
	camera_->Set();
}

void TitleScene::Draw2D()
{
	titleSceneSprite_->Draw();
	pressKeySprite_->Draw();

	for (uint8_t i = 0; i < TRANSITION_CIRCLE_NUM_; i++)
	{
		purpleCircleSprite_[i]->Draw();
	}

	if (isDrawLoading_)
	{
		purpleGroundSprite_->Draw();
		nowLoadingSprite_->Draw();
	}
}

void TitleScene::BeforeTransition()
{
	if (player2TransHomePositionTimer_.GetActive() == true)
	{
		player_->position_ -= TRANSITION_PLAYER_MOVE_;
	}
	else
	{
		if (isWait_ == false)
		{
			waitTimer_.Start(DEPARTURE_WAIT_TIME_);
			isWait_ = true;
		}
		isTransHomePos_ = true;
	}

	// 回転する演出
	player_->rotation_ = reciprocalNeedRotation_ +
		needRotation_ * Easing::Out(player2TransHomePositionTimer_.GetTimeRate(), kEASING_PLAYER_ROT_);

	if (waitTimer_.GetActive() == false)
	{
		isWait_ = false;
	}
	if (isTransHomePos_ == true && isWait_ == false)
	{
		isDeparture = true;
	}
	if (isDeparture == true)
	{
		if (isTransition_ == false)
		{
			player_->position_ += BEFORE_TRANSITION_PLAYER_MOVE_;
		}
	}

	if (player_->position_.z >= TRANSITION_CHANGE_PLAYER_POS_Z_)
	{
		if (isTransition_ == false)
		{
			circleScaleChangeAllTimer_.Start(CIRCLE_CHANGE_ANIME_ALL_TIME_);
			circleScaleChangeTimer_[spriteCount_].Start(CIRCLE_CHANGE_ANIME_MAX_TIME_);
			isTransition_ = true;
		}
	}
}

void TitleScene::Transition()
{
	isBeforeTransition_ = false;
	
	if (isDrawLoading_ == true)
	{
		endAnimation_ = true;
	}

	for (uint8_t i = 0; i < CIRCLE_SCALE_CHANGE_TIMER_NUM_; i++)
	{
		if (circleScaleChangeTimer_[i].GetActive())
		{
			timerCounts_ = i;
		}

		if (circleScaleChangeTimer_[i].GetTime() == CIRCLE_CHANGE_DELAY_TIME_)
		{
			if (timerCounts_ == i)
			{
				if (!circleScaleChangeTimer_[CIRCLE_SCALE_CHANGE_TIMER_NUM_ - 1].GetActive())
				{
					circleScaleChangeTimer_[i + 1].Start(CIRCLE_CHANGE_ANIME_MAX_TIME_);
				}
			}
		}
	}

	if (!circleScaleChangeAllTimer_.GetActive())
	{
		isDrawLoading_ = true;
	}

	for (int i = 0; i < TRANSITION_CIRCLE_XY_NUM_.x; i++)
	{
		for (int j = 0; j < TRANSITION_CIRCLE_XY_NUM_.y; j++)
		{
			purpleCircleSprite_[spriteCount_]->scale_ = {
			Easing::In(circleScaleChangeTimer_[i].GetTimeRate(), kEASING_CIRCLE_SCALE_),
			Easing::In(circleScaleChangeTimer_[i].GetTimeRate(), kEASING_CIRCLE_SCALE_) };
			spriteCount_++;
		}
	}
	spriteCount_ = INIT_SPRITE_NUM_;
}

void TitleScene::PossibleStartGame()
{
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		if (isBeforeTransition_ == false)
		{ 
			player2TransHomePositionTimer_.Start(TO_TRANS_HOME_POSITION_TIME_);

			needRotation_ = { 0,
				UsaMath::DegreesToRadians(FULL_CIRCLE_) - player_->rotation_.y, 0 };

			reciprocalNeedRotation_ = { 0, player_->rotation_.y, 0 };

			isBeforeTransition_ = true;
			isTransHomePos_ = false;
		}
	}

	if (endAnimation_ == true)
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}