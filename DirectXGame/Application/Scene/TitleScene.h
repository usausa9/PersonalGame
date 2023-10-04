#pragma once
#include "IScene.h"

#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "Object3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"

#include "Skydome.h"
#include "TimeData.h"

class CollisionManager;

class TitleScene : public IScene
{
public:
	// 初期化
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 3D描画
	void Draw3D() override;

	// パーティクル描画
	void DrawParticle() override;

	// 2D描画
	void Draw2D() override;

	// ゲームシーンへ
	void PossibleStartGame();

private:
	// カメラ
	Camera* camera_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	// プレイヤー,敵
	std::list<std::unique_ptr<Enemy>> enemys_;

	// シーン切り替えフラグ
	bool isStart_ = false;

	// タイトルスプライト
	TextureIndex titleTex_ = {};
	TextureIndex titleSceneTex_ = {};
	TextureIndex pressKeyTex_ = {};
	unique_ptr<Sprite> titleSprite_ = nullptr;
	unique_ptr<Sprite> titleSceneSprite_ = nullptr;
	unique_ptr<Sprite> pressKeySprite_ = nullptr;
	const Vector2 TITLE_BASE_POS_ = { 640, 360 };
	const Vector2 TITLE_SCENE_BASE_POS_ = { 640, 240 };
	const Vector2 PRESS_KEY_BASE_POS_ = { 640, 480 };

	// プレスキーの大きさのスカラー
	Vector2 kPressKeySpriteSize_ = {1.f, 1.f};

	// プレスキーのアニメーション用
	TimeData pressAnimeTimer_[2];
	const float PRESS_ANIME_MAX_TIMER_ = 37.f;
	uint8_t nowActiveTimer_ = 0;

	// 天球
	unique_ptr<Skydome> skydome_ = nullptr;
};