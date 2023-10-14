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

enum class TitlePhase : uint8_t
{
	IDLE = 0x0000,
	MOB = 0x0001,
	BOSS = 0x0002,
};

class GameScene : public IScene
{
private:
	// 衝突マネージャ
	CollisionManager* collisionManager_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	// プレイヤー,敵
	unique_ptr<Player> player_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;

	// 敵機モデル
	OBJModel enemyModel_[2];

	// 天球
	unique_ptr<Skydome> skydome_ = nullptr;

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