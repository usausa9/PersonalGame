#pragma once

/**
 * @file	IScene.h
 * @brief	仮想シーン
**/

// 前方宣言
class SceneManager;

class IScene
{
public:
	// 初期化
	virtual void Initialize() = 0;

	// 終了
	virtual void Finalize() = 0;

	// 更新処理
	virtual void Update() = 0;

	// 3D描画
	virtual void Draw3D() = 0;

	// パーティクル描画
	virtual void DrawParticle() = 0;

	// 2D描画
	virtual void Draw2D() = 0;

public: // メンバ関数
	virtual ~IScene() = default;

	// シーンマネージャーのセット
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:
	// シーンマネージャ (借りもの)
	SceneManager* sceneManager_ = nullptr;
};