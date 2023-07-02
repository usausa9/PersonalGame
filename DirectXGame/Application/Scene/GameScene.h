#pragma once
#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"

#include "FBXObject3D.h"
#include "Object3D.h"
#include "Camera.h"

#include "OBJModel.h"
#include "FBXModel.h"

#include "FbxLoader.h"

class GameScene
{
private:
	// テクスチャ

	// スプライト

	// パーティクル

	// OBJモデル
	OBJModel vicviper;

	// Object3D
	Object3D playerObj;

	// FBX

	// カメラ
	Camera* camera = new Camera;

	unique_ptr<Player> player = nullptr;
	

	float velocity = 0.7f;

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