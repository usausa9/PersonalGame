#pragma once
#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "PowerUpItem.h"

#include "Object3D.h"
#include "FBXObject3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"
#include "FBXModel.h"

#include "Skydome.h"

#include "FbxLoader.h"

class CollisionManager;

class GameScene
{
private:
	// 衝突マネージャ
	CollisionManager* collisionManager = nullptr;
	
	// カメラ
	Camera* camera = nullptr;
	RailCamera* railCamera = nullptr;

	// プレイヤー,敵
	unique_ptr<Player> player = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys;

	// 天球
	unique_ptr<Skydome> skydome = nullptr;

public:
	// 初期化
	void Initialize();

	// 終了処理
	void Finalize();

	// 毎フレーム更新
	void Update();

	// 3D描画
	void Draw3D();

	// パーティクル描画
	void DrawParticle();

	// 2D描画
	void Draw2D();

	// 敵
	void EnemySpawn();
};