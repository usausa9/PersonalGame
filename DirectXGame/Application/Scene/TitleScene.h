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

enum class Phase : uint8_t
{
	STRAIGHT = 0x0001,
	UP = 0x0002,
};

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

	// シーン遷移演出
	void Transition();

	// ゲームシーンへ
	void PossibleStartGame();

private:
	// カメラ
	Camera* camera_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	// プレイヤー,敵
	std::list<std::unique_ptr<Enemy>> enemys_;

	// シーン切り替えフラグ
	bool isTransition_ = false;
	bool endAnimation_ = false;

	// タイトルスプライト
	TextureIndex titleTex_ = {};
	TextureIndex titleSceneTex_ = {};
	TextureIndex pressKeyTex_ = {};
	unique_ptr<Sprite> titleSprite_ = nullptr;
	unique_ptr<Sprite> titleSceneSprite_ = nullptr;
	unique_ptr<Sprite> pressKeySprite_ = nullptr;
	const Vector2 TITLE_BASE_POS_ = { 640, 360 };
	const Vector2 TITLE_SCENE_BASE_POS_ = { 640, 200 };
	const Vector2 PRESS_KEY_BASE_POS_ = { 640, 520 };

	// プレスキーの大きさのスカラー, 基準値
	Vector2 kPressKeySpriteSize_ = { 1.f, 1.f };
	const float PRESS_KEY_HOME_SIZE_ = 0.8f;

	// アニメーション用
	uint8_t nowActiveTimer_ = 0;
	TimeData pressAnimeTimer_[2];
	TimeData playerRotateTimer_;
	TimeData waitTimer_;
	const float PRESS_ANIME_MAX_TIME_ = 37.f;
	const float PLAYER_ROTATE_ANIME_MAX_TIME_ = 360.f;
	const Vector3 PLAYER_HOME_POSITION_ = { 0, -1, 22 };
	const int8_t PLAYER_HOME_ROTATION_X_ = -5;
	const float TO_TRANS_HOME_POSITION_TIME_ = 28.f;
	const float DEPARTURE_WAIT_TIME_ = 15.f;

	bool isTransHomePos_ = false;
	bool isWait_ = false;
	bool isDeparture = false;
	TimeData player2TransHomePositionTimer_;

	Vector3 needRotation_ = { 0, 0, 0 };
	Vector3 reciprocalNeedRotation_ = { 0, 0, 0 };

	// プレイヤー,敵
	unique_ptr<Player> player_ = nullptr;

	// 天球
	unique_ptr<Skydome> skydome_ = nullptr;
};