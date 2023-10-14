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
	railCamera_->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// 天球
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();

	// 天球の行列更新
	skydome_->Update();

	// プレイヤー初期化
	player_ = make_unique<Player>();
	player_.get()->Initialize();
	player_->position_ = { PLAYER_HOME_POSITION_ };
	// タイトルスプライト
	titleTex_ = TextureManager::Load(L"Resources/Sprites/title.png");
	titleSprite_ = make_unique<Sprite>(titleTex_);
	titleSprite_->position_ = TITLE_BASE_POS_;

	titleSceneTex_ = TextureManager::Load(L"Resources/Sprites/title_scene.png");
	titleSceneSprite_ = make_unique<Sprite>(titleSceneTex_);
	titleSceneSprite_->position_ = TITLE_SCENE_BASE_POS_;

	pressKeyTex_ = TextureManager::Load(L"Resources/Sprites/press_key.png");
	pressKeySprite_ = make_unique<Sprite>(pressKeyTex_);
	pressKeySprite_->position_ = PRESS_KEY_BASE_POS_;

	nowActiveTimer_ = 0;
	isTransition_ = false;
	endAnimation_ = false;

	needRotation_ = { 0, 0, 0 };
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

	if (isTransition_ == true)
	{
		Transition();
		player2TransHomePositionTimer_.Update();
		waitTimer_.Update();
	}

	// プレイヤーの更新
	if (playerRotateTimer_.GetActive() == false && isTransition_ == false)
	{
		playerRotateTimer_.Start(PLAYER_ROTATE_ANIME_MAX_TIME_);
	}
	
	player_->SetParent(railCamera_->GetObject3d());
	player_->Update();

	// カメラをレールカメラのものへ
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// pressKEYのアニメーションタイマー更新
	if (nowActiveTimer_ == 0 && !pressAnimeTimer_[1].GetActive())
	{
		pressAnimeTimer_[0].Start(PRESS_ANIME_MAX_TIME_);
		nowActiveTimer_ = 1;
	}
	else if (nowActiveTimer_ == 1 && !pressAnimeTimer_[0].GetActive())
	{
		pressAnimeTimer_[1].Start(PRESS_ANIME_MAX_TIME_);
		nowActiveTimer_ = 0;
	}

	if (isTransition_ == false)
	{
		player_->rotation_ = { UsaMath::DegreesToRadians(PLAYER_HOME_ROTATION_X_),
			 UsaMath::DegreesToRadians(playerRotateTimer_.GetTime()), 0 };

		playerRotateTimer_.Update();
		pressAnimeTimer_[0].Update();
		pressAnimeTimer_[1].Update();
	}
	
	if (nowActiveTimer_ == 1)
	{
		kPressKeySpriteSize_ = {
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[0].GetTimeRate()) / 3),
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[0].GetTimeRate()) / 3) };
	}
	else
	{
		kPressKeySpriteSize_ = {
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[1].GetDisTimeRate()) / 3),
		PRESS_KEY_HOME_SIZE_ + (Easing::In(pressAnimeTimer_[1].GetDisTimeRate()) / 3) };
	}

	pressKeySprite_->scale_ = { kPressKeySpriteSize_ };

	// タイトルスプライト更新
	titleSprite_->Update();
	titleSceneSprite_->Update();
	pressKeySprite_->Update();
}

void TitleScene::Draw3D()
{
	// カメラセット
	camera_->Set();

	// 天球描画
	skydome_->Draw();

	// プレイヤー描画
	player_->Draw();

}

void TitleScene::DrawParticle()
{
	// カメラセット
	camera_->Set();
}

void TitleScene::Draw2D()
{
	//titleSprite_->Draw();
	titleSceneSprite_->Draw();
	pressKeySprite_->Draw();
}

void TitleScene::Transition()
{
	if (player2TransHomePositionTimer_.GetActive() == true)
	{
		player_->position_ -= { 0, 0, 0.25f };
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

	player_->rotation_ = reciprocalNeedRotation_ +
		needRotation_ * Easing::Out(player2TransHomePositionTimer_.GetTimeRate(), 4);

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
		player_->position_ += { 0, 0, 5 };
	}
}

void TitleScene::PossibleStartGame()
{
	if (Key::Trigger(DIK_SPACE) && isTransition_ == false)
	{
		player2TransHomePositionTimer_.Start(TO_TRANS_HOME_POSITION_TIME_);

		needRotation_ = {0,
			UsaMath::DegreesToRadians(360) - player_->rotation_.y, 0};

		reciprocalNeedRotation_ = { 0, player_->rotation_.y, 0 };

		isTransition_ = true;
		isTransHomePos_ = false;
	}

	if (endAnimation_ == true)
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}