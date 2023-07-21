#include "Player.h"
#include "SphereCollider.h"
#include "WinAPI.h"

using namespace Input;

// 初期化
void Player::Initialize()
{
	// 自機モデル等読み込み
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	reticleModel = OBJModel::LoadFromOBJ("ICO");

	// レティクルスプライト割り当て
	reticleTex = TextureManager::Load(L"Resources/Sprites/reticle.png");
	reticleSp = make_unique<Sprite>(reticleTex);
	reticleSp->position = reticlePos;
	
	// 自機の行列初期化
	rotation = { -7 * (UsaMath::u_PI / 180), 0, 0 };
	position = { 0, 0, 20 };
	InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	objModel = &playerModel;
	reticleObj.objModel = &reticleModel; 
	reticleObj.position = { 0, 0, 50 };
	reticleObj.InitializeObject3D();

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }),radius));

	playerObj.objModel = &playerModel;
}

// 更新
void Player::Update()
{
	// 入力からの移動処理
	Move();

	// 行列更新 必ず呼び出す
	UpdateObject3D();

	// レティクルの更新
	reticleUpdate();

	// 弾発射 + 更新
	Shot();
	if (bullet) 
	{
		bullet->Update();
	}

	// 行列更新 必ず呼び出す
	playerObj.UpdateObject3D();
}

// 描画
void Player::Draw()
{
	// オブジェ描画
	playerObj.DrawObject3D();

	// 弾描画
	if (bullet)
	{
		bullet->Draw();
	}
}

void Player::OnCollision(const CollisionInfo& info)
{

}

void Player::reticleUpdate()
{
	// ビューポート行列
	Matrix4 matViewPort;
	matViewPort.Identity();
	matViewPort.m[0][0] =   WinAPI::Get()->width  / 2.0f;
	matViewPort.m[1][1] = -(WinAPI::Get()->height / 2.0f);
	matViewPort.m[3][0] =   WinAPI::Get()->width  / 2.0f;
	matViewPort.m[3][1] =   WinAPI::Get()->height / 2.0f;

	// カメラ行列との合成
	Matrix4 matViewProjectionViewPort = GetViewProjection() * matViewPort;

	// 画面上のレティクル座標を動かす
	Vector2 reticleMoveVel = { 0, 0 };
	float reticleSpd = 4.0f;

	if (Key::Trigger(DIK_LEFT)) 
	{
		reticleMoveVel.x = -reticleSpd;
	}
	else if (Key::Trigger(DIK_RIGHT)) 
	{
		reticleMoveVel.x = reticleSpd;
	}

	if (Key::Trigger(DIK_UP)) 
	{
		reticleMoveVel.y = -reticleSpd;
	}
	else if (Key::Trigger(DIK_DOWN))
	{
		reticleMoveVel.y = reticleSpd;
	}

	reticlePos += reticleMoveVel;

	// 座標をスプライトにセット
	reticleSp->position = reticlePos;

	// 合成行列の生成
	Matrix4 matInverseVBV = matViewProjectionViewPort;
	matInverseVBV = Matrix4::Inverse(matInverseVBV);

	// スクリーン座標
	Vector3 posNear = { reticlePos.x, reticlePos.y, 0 };
	Vector3 posFar = { reticlePos.x, reticlePos.y, 1 };

	// スクリーン座標系からワールド座標系へ
	posNear = Matrix4::TransformDivW(posNear, matInverseVBV);
	posFar = Matrix4::TransformDivW(posFar, matInverseVBV);

	// マウスレイの方向
	Vector3 direction = posFar - posNear;
	direction.Normalize();

	// カメラからレティクルの距離
	const float distanceReticle = 100.0f;
	reticleObj.position = posNear + direction * distanceReticle;

	reticleObj.UpdateObject3D();
	reticleSp->Update();
}

void Player::DrawUI()
{
	reticleSp->Draw();
}

// 入力受け付け + 移動
void Player::Move()
{
	move = { 0,0,0 };

	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * Velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * Velocity * kYMove,
		0 };
	
	playerObj.position += move;

	// 範囲制限
	playerObj.position.x = max(playerObj.position.x, -kMoveLimit.x);
	playerObj.position.y = max(playerObj.position.y, -kMoveLimit.y);
	playerObj.position.x = min(playerObj.position.x, +kMoveLimit.x);
	playerObj.position.y = min(playerObj.position.y, +kMoveLimit.y);
}

void Player::Shot()
{
	if (Key::Trigger(DIK_SPACE))
	{
		// 自機弾の毎フレーム移動
		Vector3 velocity = { 0, 0, 0 };
		velocity = reticleObj.GetWorldPosition() - Object3D::GetWorldPosition();
		velocity.Normalize();
		velocity *= kBulletSpeed;

		// 自機弾の自機からみたローカル発射位置
		Vector3 delayPos = { 0, 0.2f, 7.1f };

		// 速度ベクトルを自機の向きに合わせて回転
		velocity = velocity * matWorld;
		delayPos = delayPos * matWorld;
		
		// 自機弾を生成、初期化
		bullets.push_back(std::move(make_unique<PlayerBullet>()));
		bullets.back()->Initialize(&bulletModel, GetWorldPosition() + delayPos, velocity);
	}
}