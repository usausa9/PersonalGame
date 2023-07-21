#include "Enemy.h"
#include "SphereCollider.h"

// 初期化
void Enemy::Initialize()
{
	// 自機モデル読み込み
	enemyModel = OBJModel::LoadFromOBJ("Cube");

	// 自機の行列初期化
	rotation = { 0, 0, 0 };
	position = { 0, 0, 50 };
	InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	objModel = &enemyModel;

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
}

// 更新
void Enemy::Update()
{
	// 移動処理
	//Move();

	// 行列更新 必ず呼び出す
	UpdateObject3D();
}

// 描画
void Enemy::Draw()
{
	// オブジェ描画
	DrawObject3D();
}

void Enemy::OnCollision(const CollisionInfo& info)
{

}

// 入力受け付け + 移動
void Enemy::Move()
{
	// 移動量の初期化
	move = { 0,0,0 };

	// 移動量の加算
	position += move;
}