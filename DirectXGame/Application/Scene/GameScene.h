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
};