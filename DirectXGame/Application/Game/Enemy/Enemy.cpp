/**
 * @file	Enemy.cpp
 * @brief	敵のデータ保持や初期化/更新/描画を行うもの (Managerに近しい)
**/

#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"
#include "TimeData.h"

#include <fstream>
#include <sstream>

bool Enemy::sIsPowerUp_ = false;

// 初期化
void Enemy::Initialize(std::vector<Vector3>& points, uint8_t enemyKind)
{
	if (enemyKind == uint8_t(EnemyKinds::NORMAL))
	{
		// 自機モデル読み込み
		enemyModel_ = OBJModel::LoadFromOBJ("Cube");
	}
	else if (enemyKind == uint8_t(EnemyKinds::POWER))
	{
		// 自機モデル読み込み
		enemyModel_ = OBJModel::LoadFromOBJ("ICO");
	}

	// 敵の種類指定
	enemyKind_ = enemyKind;

	// 自機の行列初期化
	rotation_ = { 0, 0, 0 };
	position_ = { 0, 0, 0 };
	InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	objModel_ = &enemyModel_;

	// 軌道制御点の設定
	trajectory_.SetPositions(points);

	// コライダーの追加
	float radius = 1.3f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider_->SetAttribute(COLLISION_ATTR_ENEMYS);
}

// 更新
void Enemy::Update(const Matrix4& cameraMatrix)
{
	// 強化可能判定を一時的にfalseへ
	sIsPowerUp_ = false;

	// 敵の軌道更新
	trajectory_.Update();

	// 座標をカメラ基準に
	Vector3 cameraPos = { cameraMatrix.m[3][0],cameraMatrix.m[3][1], cameraMatrix.m[3][2] };
	position_ = Matrix4::Transform(trajectory_.GetNowPosition(), cameraMatrix) + cameraPos;

	// 行列更新 必ず呼び出す
	UpdateObject3D();
}

// 描画
void Enemy::Draw()
{
	// オブジェ描画
	DrawObject3D();

	if (isAlive_ != true)
	{
		return;
	}
}

void Enemy::Spawn()
{
	// 敵発生
	trajectory_.MoveStart(600, true);
}

void Enemy::OnCollision([[maybe_unused]] const CollisionInfo& info)
{
	if (enemyKind_ == uint8_t(EnemyKinds::POWER))
	{
		// 判定をtrueに
		sIsPowerUp_ = true;
	}

	// 衝突判定により消滅
	isAlive_ = false;
}