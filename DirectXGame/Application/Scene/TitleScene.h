#pragma once
/**
 * @file	TitleScene.h
 * @brief	タイトルシーン全体のデータ保持や初期化/更新/描画を行うもの
**/

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

enum PressKeyState
{
	EXPAND,
	SHRINK
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

private: // メンバ関数 (クラス内でのみ使用するもの)
	// シーン遷移演出
	void BeforeTransition();
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
	bool isBeforeTransition_ = false;
	bool isTransition_ = false;
	bool endAnimation_ = false;

	// タイトルスプライト
	TextureIndex titleSceneTex_ = {};
	TextureIndex pressKeyTex_ = {};
	TextureIndex purpleCircleTex_ = {};
	TextureIndex purpleGroundTex_ = {};
	TextureIndex nowLoadingTex_ = {};

	unique_ptr<Sprite> purpleCircleSprite_[28];
	unique_ptr<Sprite> titleSceneSprite_ = nullptr;
	unique_ptr<Sprite> pressKeySprite_ = nullptr;
	unique_ptr<Sprite> purpleGroundSprite_ = nullptr;
	unique_ptr<Sprite> nowLoadingSprite_ = nullptr;
	
	const Vector2 TRANSITION_CIRCLE_XY_NUM_ = { 7, 4 };
	const Vector2 TRANSITION_CIRCLE_BASE_POS_ = { 91.43f, 90 };
	const Vector2 TRANSITION_CIRCLE_BASE_SIZE_ = { 182.86f, 180 };
	const Vector2 TRANSITION_CIRCLE_INIT_SIZE_ = { 0.f, 0.f };
	const uint8_t TRANSITION_CIRCLE_NUM_ = 28;
	
	const Vector2 TITLE_BASE_POS_ = { 640, 360 };
	const Vector2 TITLE_SCENE_BASE_POS_ = { 640, 200 };
	const Vector2 PRESS_KEY_BASE_POS_ = { 640, 520 };
	uint8_t spriteCount_ = 0;
	const uint8_t INIT_SPRITE_NUM_ = 0;
	float transitionCircleScale_ = 0.f;
	const float INIT_TRANSITION_CIRCLE_SCALE_ = 0.f;

	// プレスキーの大きさのスカラー, 基準値
	Vector2 kPressKeySpriteSize_ = { 1.f, 1.f };
	const float PRESS_KEY_HOME_SIZE_ = 0.8f;
	// プレスキーの初期サイズ
	const Vector2 kPRESS_KEY_SPRITE_SIZE_ = { 1.f, 1.f };

	// レールカメラの初期座標と回転角
	const Vector3 CAMERA_INIT_POS_ = { 0, 0, -20.0f };
	const Vector3 CAMERA_INIT_ROT_ = { 0, 0, 0 };

	// アニメーションタイマー関連
	uint8_t nowActiveTimer_ = 0;
	const uint8_t INIT_TIMER_COUNT_ = 0;
	TimeData pressAnimeTimer_[2];
	TimeData playerRotateTimer_;
	TimeData waitTimer_;
	TimeData player2TransHomePositionTimer_;
	const float PRESS_ANIME_MAX_TIME_ = 37.f;
	const float PLAYER_ROTATE_ANIME_MAX_TIME_ = 360.f;
	const float TO_TRANS_HOME_POSITION_TIME_ = 28.f;
	const float DEPARTURE_WAIT_TIME_ = 15.f;

	// アニメーションのフラグ
	bool isTransHomePos_ = false;
	bool isWait_ = false;
	bool isDeparture = false;
	bool isDrawLoading_ = false;
	bool isNextCircleChange_ = true;
	
	const uint8_t PRESS_ANIME_TIMER_NUM_ = 2;
	const Vector3 PLAYER_HOME_POSITION_ = { 0, -1, 22 };
	const int8_t PLAYER_HOME_ROTATION_X_ = -5;
	
	// イージングのスカラー値
	const uint8_t kEASING_PLAYER_ROT_ = 4;
	const uint8_t kEASING_PLESS_SCALE_ = 3;
	const float kEASING_CIRCLE_SCALE_ = 2.2f;

	// プレイヤーの回転アニメーション
	const Vector3 ROT_INIT_ = { 0, 0, 0 };
	Vector3 needRotation_ = { 0, 0, 0 };
	Vector3 reciprocalNeedRotation_ = { 0, 0, 0 };
	const float FULL_CIRCLE_ = 360.f;
	const Vector3 TRANSITION_PLAYER_MOVE_ = { 0, 0, 0.25f };
	const Vector3 BEFORE_TRANSITION_PLAYER_MOVE_ = { 0, 0, 5 };
	const float TRANSITION_CHANGE_PLAYER_POS_Z_ = 200.0f;

	// トランジション用
	const float CIRCLE_CHANGE_ANIME_MAX_TIME_ = 25.f;
	const float CIRCLE_CHANGE_DELAY_TIME_ = 5.f;
	const float CIRCLE_CHANGE_ANIME_ALL_TIME_ = 66.f;
	uint8_t timerCounts_ = 0;
	TimeData circleScaleChangeAllTimer_;
	const uint8_t CIRCLE_SCALE_CHANGE_TIMER_NUM_ = 7;
	TimeData circleScaleChangeTimer_[7];

	// プレイヤーの基準座標
	const Vector2 STANDARD_PLATER_POS_ = { 0, 0 };

	// プレイヤー,敵
	unique_ptr<Player> player_ = nullptr;

	// 天球
	unique_ptr<Skydome> skydome_ = nullptr;
};