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

	// タイトルスプライト
	titleTex_ = TextureManager::Load(L"Resources/Sprites/title.png");
	titleSprite_ = make_unique<Sprite>(titleTex_);
	titleSprite_->position_ = TITLE_BASE_POS_;

	titleSceneTex_ = TextureManager::Load(L"Resources/Sprites/title_scene.png");
	titleSceneSprite_ = make_unique<Sprite>(titleSceneTex_);
	titleSceneSprite_->position_ = TITLE_SCENE_BASE_POS_;

	plessKeyTex_ = TextureManager::Load(L"Resources/Sprites/pless_key.png");
	plessKeySprite_ = make_unique<Sprite>(plessKeyTex_);
	plessKeySprite_->position_ = PLESS_KEY_BASE_POS_;

	plessAnimeTimer.Start(float)
}

void TitleScene::Finalize()
{
	delete railCamera_;
}

void TitleScene::Update()
{
	// SPACEキーを押したらゲームシーンに。
	isStart_ = false;

	if (Key::Trigger(DIK_SPACE))
	{
		isStart_ = true;
	}

	// スタートフラグが立ったらゲームシーンへ
	PossibleStartGame();

	// DirectX毎フレーム処理(更新処理) ここから
	railCamera_->Update();

	// カメラをレールカメラのものへ
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// タイトルスプライト更新
	titleSprite_->Update();
	titleSceneSprite_->Update();
	plessKeySprite_->Update();
}

void TitleScene::Draw3D()
{
	// カメラセット
	camera_->Set();
	
	// 天球描画
	skydome_->Draw();
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
	plessKeySprite_->Draw();
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}