#pragma once
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

	// プレイヤー,敵
	unique_ptr<Player> player_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;

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
	void EnemySpawn(uint8_t enemyKind);

	// ファイル読み込み
	void LoadCsvFile();

	// 読み込んだデータから敵の種類付け等
	void LoadData();

	// 敵発生コマンド
	std::stringstream enemyData_;

	// 敵発生待ち時間
	TimeData waitTimer_;

	// 敵待ちフラグ
	bool isStandBy_ = false;
};