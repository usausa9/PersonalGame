#include "MyGame.h"
#include "UsaFramework.h"
#include "SceneFactory.h"

void MyGame::Initialize()
{
	// 基底クラスの初期化処理
	UsaFramework::Initialize();

	// シーンファクトリーを生成しマネージャにセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	// 基底クラスの終了処理
	UsaFramework::Finalize();
}

void MyGame::Update()
{
	// 基底クラスの更新処理
	UsaFramework::Update();
}

void MyGame::Draw()
{
	// 基底クラスの描画処理
	UsaFramework::Draw();
}