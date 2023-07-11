#include "Player.h"

using namespace Input;

// 初期化
void Player::Initialize()
{
	// 自機モデル読み込み
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	
	// 自機の行列初期化
	playerObj.rotation = { -20 * (UsaMath::u_PI / 180), 0, 0 };
	playerObj.InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	playerObj.objModel = &playerModel;

	// 自機弾モデル読み込み
	bulletModel = OBJModel::LoadFromOBJ("ICO");
}

// 更新
void Player::Update()
{
	// 消滅フラグが立った弾を削除
	bullets.remove_if([](unique_ptr<PlayerBullet>& bullet)
	{
		return bullet->IsDead();
	});

	// 入力からの移動処理
	Move();

	// 行列更新 必ず呼び出す
	playerObj.UpdateObject3D();

	// 弾発射 + 更新
	Shot();
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update();
	}
}

// 描画
void Player::Draw()
{
	// オブジェ描画
	playerObj.DrawObject3D();

	// 弾描画
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Draw();
	}
}

// 入力受け付け + 移動
void Player::Move()
{
	// 移動量の初期化
	move = { 0,0,0 };

	// WASD入力での移動
	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * velocity * kYMove,
		0 };

	// GamePadでの移動
	move += {
		Pad::GetLStick().x * velocity,
		Pad::GetLStick().y * velocity * kYMove,
		0 };

	// 移動量の加算
	playerObj.position += move;

	// 範囲制限
	playerObj.position.x = max(playerObj.position.x, -kMoveLimit.x);
	playerObj.position.y = max(playerObj.position.y, -kMoveLimit.y);
	playerObj.position.x = min(playerObj.position.x, +kMoveLimit.x);
	playerObj.position.y = min(playerObj.position.y, +kMoveLimit.y);
}

void Player::Shot()
{
	// スペースキーのトリガー入力を受け付けた場合
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		// 自機弾の毎フレーム移動
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転
		velocity = velocity * playerObj.matWorld;
		
		// 自機弾を生成、初期化
		bullets.push_back(std::move(make_unique<PlayerBullet>()));
		bullets.back()->Initialize(&bulletModel, playerObj.position + delayPos, velocity);
	}
}