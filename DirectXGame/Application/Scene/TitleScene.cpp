#include "TitleScene.h"
#include "SceneManager.h"

#include "CollisionManager.h"

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

	// レールカメラ初期化
	railCamera = new RailCamera();
	railCamera->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// 天球初期化
	skydome = make_unique<Skydome>();
	skydome.get()->Initialize();
}

void TitleScene::Finalize()
{
	delete railCamera;
}

void TitleScene::Update()
{
	// 取りあえず仮でTキーを押したらゲームシーンに。
	isStart = false;

	if (Key::Trigger(DIK_T))
	{
		isStart = true;
	}

	// スタートフラグが立ったらゲームシーンへ
	PossibleStartGame();

	// DirectX毎フレーム処理(更新処理) ここから
	railCamera->Update();

	// 天球の行列更新
	skydome->Update();

	// カメラをレールカメラのものへ
	camera = railCamera->GetCamera();
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

void TitleScene::PossibleStartGame()
{
	if (isStart == true)
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}