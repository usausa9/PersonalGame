#include "Player.h"

using namespace Input;

// 初期化
void Player::Initialize()
{
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	bulletModel = OBJModel::LoadFromOBJ("ICO");

	bullet->Initialize(&bulletModel, playerObj.position);
	bullet->bulletObj.objModel = &bulletModel;
	
	playerObj.rotation = { -20 * (UsaMath::u_PI / 180), 0, 0 };
	playerObj.InitializeObject3D();

	playerObj.objModel = &playerModel;
}

// 更新
void Player::Update()
{
	// 入力からの移動処理
	Move();

	// 弾発射処理 / 更新
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
		// 自機弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(&bulletModel, playerObj.position);

		// 弾を登録
		bullet = newBullet;
	}
}