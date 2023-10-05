#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 次シーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::Update()
{
	// 切り替え機構

	// 次シーンの予約があるなら
	if (nextScene_)
	{
		// 旧シーンの終了
		if (currentScene_)
		{
			// 現在シーンの終了処理
			currentScene_->Finalize();
			delete currentScene_;
		}

		// シーン切り替え
		currentScene_ = nextScene_;
		nextScene_ = nullptr;

		// シーンマネージャをセット
		currentScene_->SetSceneManager(this);

		// 次シーンの初期化
		currentScene_->Initialize();
	}

	// 実行中シーンを更新する
	currentScene_->Update();
}

void SceneManager::Draw3D()
{
	// 3D描画
	currentScene_->Draw3D();
}

void SceneManager::DrawParticle()
{
	// パーティクル描画
	currentScene_->DrawParticle();
}

void SceneManager::Draw2D()
{
	// 2D描画
	currentScene_->Draw2D();
}

SceneManager::~SceneManager()
{
	// 最後のシーンの終了と解放
	currentScene_->Finalize();
	delete currentScene_;
}