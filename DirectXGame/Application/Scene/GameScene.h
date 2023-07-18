#pragma once
#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"

#include "Object3D.h"
#include "FBXObject3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"
#include "FBXModel.h"

#include "Skydome.h"

#include "FbxLoader.h"

class GameScene
{
private:
	// カメラ
	Camera* camera = nullptr;
	RailCamera* railCamera = nullptr;

	// プレイヤー
	unique_ptr<Player> player = nullptr;
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
};