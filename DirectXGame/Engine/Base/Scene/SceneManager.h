#pragma once
#include "AbstractSceneFactory.h"

// シーン管理
class SceneManager
{
public:
	// シングルトンインスタンスを取得
	static SceneManager* GetInstance();

	// 次シーンの予約
	void ChangeScene(const std::string& sceneName);

	// シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
private:
	// 現在のシーン
	IScene* currentScene_ = nullptr;
	IScene* nextScene_ = nullptr;

	// シーン工場 (借りもの)
	AbstractSceneFactory* sceneFactory_ = nullptr;

public: // メンバ関数
	// 更新処理
	void Update();

	// 3D描画
	void Draw3D();

	// パーティクル描画
	void DrawParticle();

	// 2D描画
	void Draw2D();
	
	// デストラクタ
	~SceneManager();
};