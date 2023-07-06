#include "Player.h"

using namespace Input;

// 初期化
void Player::Initialize()
{
	playerModel = OBJModel::LoadFromOBJ("vicviper");

	playerObj.position = { 0, 0, 0 };
	playerObj.scale = { 1, 1, 1 };
	playerObj.rotation = { -20 * (UsaMath::u_PI / 180), 0, 0 };
	playerObj.InitializeObject3D();

	playerObj.objModel = &playerModel;
}

// 更新
void Player::Update()
{
	// 入力からの移動処理
	Move();

	// 行列更新 必ず呼び出す
	playerObj.UpdateObject3D();
}

// 描画
void Player::Draw()
{
	// オブジェ描画
	playerObj.DrawObject3D();
}

// 移動
void Player::Move()
{
	move = { 0,0,0 };

	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * Velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * Velocity * 9 / 13,
		0 };
	
	playerObj.position += move;
}