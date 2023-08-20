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

	// タイトルスプライト
	titleTex_ = TextureManager::Load(L"Resources/Sprites/title.png");
	titleSprite_ = make_unique<Sprite>(titleTex_);
	titleSprite_->position_ = SPRITE_BASE_POS_;
}

void TitleScene::Finalize()
{
	delete railCamera_;
}

void TitleScene::Update()
{
	// 取りあえず仮でTキーを押したらゲームシーンに。
	isStart_ = false;

	if (Key::Trigger(DIK_T))
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

	// タイトルスプライト更新3
	titleSprite_->Update();
}

void TitleScene::Draw3D()
{
	// カメラセット
	camera_->Set();
}

void TitleScene::DrawParticle()
{
	// カメラセット
	camera_->Set();

	// パーティクルオブジェ描画
}

void TitleScene::Draw2D()
{
	titleSprite_->Draw();
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}