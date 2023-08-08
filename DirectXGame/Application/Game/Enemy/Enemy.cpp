#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

// 初期化
void Enemy::Initialize(std::vector<Vector3>& points)
{
	// 自機モデル読み込み
	enemyModel = OBJModel::LoadFromOBJ("Cube");

	// 自機の行列初期化
	rotation = { 0, 0, 0 };
	position = { 0, 0, 0 };
	InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	objModel = &enemyModel;

	// 軌道制御点の設定
	trajectory.SetPositions(points);

	// コライダーの追加
	float radius = 1.3f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

// 更新
void Enemy::Update(const Matrix4& cameraMatrix, const Vector3& playerWorldPosition)
{
	// 敵の軌道更新
	trajectory.Update();

	// 座標をカメラ基準に
	Vector3 cameraPos = { cameraMatrix.m[3][0],cameraMatrix.m[3][1], cameraMatrix.m[3][2] };
	position = Matrix4::Transform(trajectory.GetNowPosition(), cameraMatrix) + cameraPos;

	// 移動処理
	Move();

	// 行列更新 必ず呼び出す
	UpdateObject3D();
}

// 描画
void Enemy::Draw()
{
	// オブジェ描画
	DrawObject3D();

	if (isAlive != true)
	{
		return;
	}
}

void Enemy::Spawn()
{
	// 敵発生
	trajectory.MoveStart(600, true);
}

void Enemy::OnCollision(const CollisionInfo& info)
{
	// 衝突判定により消滅
	//isAlive = false;
}

// 入力受け付け + 移動
void Enemy::Move()
{
	// 移動量の初期化
	move = { 1.5f,0,0 };


	// 移動量の加算
	position += move;

}