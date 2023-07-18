#include "Enemy.h"

// 初期化
void Enemy::Initialize()
{
	// 自機モデル読み込み
	enemyModel = OBJModel::LoadFromOBJ("vicviper");

	// 自機の行列初期化
	enemyObj.rotation = { 0, 0, 0 };
	enemyObj.position = { 0, 0, 20 };
	enemyObj.InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	enemyObj.objModel = &enemyModel;
}

// 更新
void Enemy::Update()
{
	// 入力からの移動処理
	Move();

	// 行列更新 必ず呼び出す
	enemyObj.UpdateObject3D();
}

// 描画
void Enemy::Draw()
{
	// オブジェ描画
	enemyObj.DrawObject3D();
}

// 入力受け付け + 移動
void Enemy::Move()
{
	// 移動量の初期化
	move = { 0,0,0 };


	// 移動量の加算
	enemyObj.position += move;

}