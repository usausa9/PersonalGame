#pragma once

/**
 * @file	GameScene.h
 * @brief	ゲームシーン全体のデータ保持や初期化/更新/描画を行うもの
**/

#include "IScene.h"

#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "Object3D.h"
#include "FBXObject3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"

#include "Skydome.h"

#include <iostream>
#include <string>
#include <sstream>

class CollisionManager;

class GameScene : public IScene
{
private:
	// 衝突マネージャ
	CollisionManager* collisionManager_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	const Vector3 INIT_CAMERA_POSITION_ = { 0, 0, 20.f };
	const Vector3 INIT_CAMERA_ROTATION_ = { 0, 0, 0 };

	// プレイヤー,敵
	unique_ptr<Player> player_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;
	const Vector3 INIT_PLAYER_POSITION_ = { 0, 0, -15 };

	// 敵機モデル
	OBJModel enemyModel_[2];

	// 天球
	unique_ptr<Skydome> skydome_ = nullptr;

	// スプライト
	TextureIndex purpleGroundTex_ = {};
	TextureIndex nowLoadingTex_ = {};

	unique_ptr<Sprite> purpleGroundSprite_[2] = { nullptr, nullptr };
	unique_ptr<Sprite> nowLoadingSprite_ = nullptr;

	const Vector2 BASE_POS_ = { 640, 360 };
	const float TRANSITION_MOVE_POS_ = 1800.f;

	const uint8_t PURPLE_BG_NUM_ = 2;

	TimeData groundAnimeTimer_;
	TimeData loadingAnimeTimer_;
	TimeData playerStartAnimeTimer_;
	TimeData uiAppearAnimeTimer_;
	const float GROUND_ANIME_MAX_TIME_ = 60.f;
	const float LOADING_ANIME_MAX_TIME_ = 30.f;
	const float PLAYER_START_ANIME_MAX_TIME_ = 120.f;
	const float UI_APPEAR_ANIME_MAX_TIME_ = 30.f;

	const Vector3 PLAYER_BEFORE_ANIME_POS_ = { 0, 0, -15 };
	const Vector3 PLAYER_ANIME_MOVE_ = { 0, 0, 37 };
	float playerStateUiPosY_ = 900.f;
	const float PLAYER_STATE_UI_BEFORE_POS_Y_ = 900.f;
	const float PLAYER_STATE_UI_MOVE_Y_ = PLAYER_STATE_UI_BEFORE_POS_Y_ - 667.f;

	bool isEndTransition_ = false;
	bool isEndStartAnimation_ = false;
	bool isUiAnimation_ = false;

public:
	// 初期化
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 入りのトランジションの処理
	void InTransition();

	// ゲーム前の開始演出
	void BeforeStartAnimation();

	// 3D描画
	void Draw3D() override;

	// パーティクル描画
	void DrawParticle() override;

	// 2D描画
	void Draw2D() override;

public:
	// 敵
	void EnemySpawn(uint8_t enemyKind, uint8_t trajectoryKind);

	// ファイル読み込み
	void LoadCsvFile();

	// 読み込んだデータから敵の種類付け等
	void UpdateEnemyData();

	// 敵の起動種類
	std::vector<Vector3> TrajectoryKind(uint8_t trajectoryKind);

	// 敵発生コマンド
	std::stringstream enemyData_;

	// 敵発生待ち時間
	TimeData waitTimer_;

	// 敵待ちフラグ
	bool isStandBy_ = false;
};