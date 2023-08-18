#include "TitleScene.h"
#include "SceneManager.h"

#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
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

	// 天球初期化
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();
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

	// 天球の行列更新
	skydome_->Update();

	// カメラをレールカメラのものへ
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// 全ての衝突をチェック (更新の最後)
	collisionManager_->CheckAllCollisions();
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

	// パーティクルオブジェ描画
}

void TitleScene::Draw2D()
{
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}