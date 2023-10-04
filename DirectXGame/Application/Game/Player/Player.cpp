#include "Player.h"
#include "SphereCollider.h"
#include "WinAPI.h"
#include "CollisionAttribute.h"

using namespace Input;

// 初期化
void Player::Initialize(Camera* camera)
{
	// 自機モデル読み込み
	playerModel_ = OBJModel::LoadFromOBJ("vicviper");
	reticleModel_ = OBJModel::LoadFromOBJ("sphere");

	// レティクルスプライト割り当て
	reticleTex_ = TextureManager::Load(L"Resources/Sprites/reticle.png");
	reticleSp_ = make_unique<Sprite>(reticleTex_);
	reticleSp_->position_ = reticlePos_;

	// 当たり判定デバッグ用スプライト
	aTex_ = TextureManager::Load(L"Resources/Sprites/texture.png");
	aSp_ = make_unique<Sprite>(aTex_);
	aSp_->position_ = { 100, 100 };
	
	// 自機の行列初期化
	rotation_ = { 0, 0, 0 };
	position_ = { 0, 0, 22 };
	InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	objModel_ = &playerModel_;
	reticleObj_.objModel_ = &reticleModel_;
	reticleObj_.position_ = { 0, 0, 50 };
	reticleObj_.InitializeObject3D();

	// プレイヤー状態の初期化
	state_.Initialize();

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }),radius));
	collider_->SetAttribute(COLLISION_ATTR_ALLIES);

	// 自機弾モデル読み込み
	bulletModel_ = OBJModel::LoadFromOBJ("sphere");
}

// 更新
void Player::Update()
{
	// プレイヤー状態の更新
	state_.Update();

	// 消滅フラグが立った弾を削除
	bullets_.remove_if([](unique_ptr<PlayerBullet>& bullet)
	{
		return bullet->IsDead();
	});

	// 入力からの移動処理
	Move();

	// 行列更新 必ず呼び出す
	UpdateObject3D();

	// レティクルの更新
	reticleUpdate();

	// 弾発射 + 更新
	Shot();
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}

// 描画
void Player::Draw()
{
	// オブジェ描画
	DrawObject3D();
	reticleObj_.DrawObject3D();

	// 弾描画
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
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
	Matrix4 matViewPort = Matrix4::Identity(); 
	matViewPort.m[0][0] =   WinAPI::GetInstance()->width_  / 2.0f;
	matViewPort.m[1][1] = -(WinAPI::GetInstance()->height_ / 2.0f);
	matViewPort.m[3][0] =   WinAPI::GetInstance()->width_  / 2.0f;
	matViewPort.m[3][1] =   WinAPI::GetInstance()->height_ / 2.0f;

	// カメラ行列との合成
	Matrix4 matViewProjectionViewPort = 
		Camera::GetCurrentCamera()->GetViewProjection() * matViewPort;
	
	// 画面上のレティクル座標を動かす
	Vector2 reticleMoveVel = { 0, 0 };

	if (state_.ExpandNum() == false)
	{
		reticleSp_->scale_ = { 0.9f, 0.9f };
	}
	else
	{
		reticleSp_->scale_ = { 1.1f, 1.1f };
	}

	// 自機の速さとレティクルのスピード調整
	reticleSpd_ = kReticleSpd_ * velocity_;

	if (Key::Down(DIK_A) && Key::Down(DIK_D))
	{

	}
	else if (Key::Down(DIK_A))
	{
		reticleMoveVel.x = -reticleSpd_;
	}
	else if (Key::Down(DIK_D))
	{
		reticleMoveVel.x = reticleSpd_;
	}

	if (Key::Down(DIK_W) && Key::Down(DIK_S))
	{

	}
	else if (Key::Down(DIK_W))
	{
		reticleMoveVel.y = -reticleSpd_ * kY_MOVE_RETICLE_;
	}
	else if (Key::Down(DIK_S))
	{
		reticleMoveVel.y = reticleSpd_ * kY_MOVE_RETICLE_;
	}

	reticlePos_ += reticleMoveVel;

	// レティクル座標の移動制限
	Vector2 reticlePosMin = { RETICLE_MOVE_LIMIT_, RETICLE_MOVE_LIMIT_ * 0.6f };
	Vector2 reticlePosMax = { WinAPI::GetInstance()->width_  - RETICLE_MOVE_LIMIT_,
							  WinAPI::GetInstance()->height_ - RETICLE_MOVE_LIMIT_ * 0.6f };
	
	reticlePos_.x = max(reticlePos_.x, reticlePosMin.x);
	reticlePos_.y = max(reticlePos_.y, reticlePosMin.y);
	reticlePos_.x = min(reticlePos_.x, reticlePosMax.x);
	reticlePos_.y = min(reticlePos_.y, reticlePosMax.y);

	// 座標をスプライトにセット
	reticleSp_->position_ = reticlePos_;

	// 合成行列の生成
	Matrix4 matInverseVBV = matViewProjectionViewPort;
	matInverseVBV = Matrix4::Inverse(matInverseVBV);

	// スクリーン座標
	Vector3 posNear = { reticlePos_.x, reticlePos_.y, 0 };
	Vector3 posFar = { reticlePos_.x, reticlePos_.y, 1 };

	// スクリーン座標系からワールド座標系へ
	posNear = Matrix4::TransformDivW(posNear, matInverseVBV);
	posFar = Matrix4::TransformDivW(posFar, matInverseVBV);

	// レイの方向
	Vector3 direction = posFar - posNear;
	direction.Normalize();

	// カメラからレティクルの距離
	reticleObj_.position_ = posNear + direction * DISTANCE_RETICLE_;

	reticleObj_.UpdateObject3D();
	reticleSp_->Update();
}

void Player::DrawUI()
{
	reticleSp_->Draw();
	
	// 判定時描画
	for (unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		if (bullet->IsDrawSP())
		{
			//aSp_->Draw();
		};
	}

	// 自機状態の描画
	state_.DrawUI();
}

// 入力受け付け + 移動
void Player::Move()
{
	// 移動量の初期化
	move_ = { 0,0,0 };

	// 強化に応じてスピード変化
	velocity_ = FORMERLY_SPEED_ + (SPEED_UP_RATE_ * state_.SpeedUpNum());

	// WASD入力での移動
	move_ += {
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * velocity_,
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * velocity_ * kY_MOVE_,
		0 };

	/*rev = {
		(Key::Down(DIK_S) - Key::Down(DIK_W)) * rotateRev,
		0,
		((Key::Down(DIK_A) - Key::Down(DIK_D)) * rotateRev) * kYRotate };*/

	// GamePadでの移動
	move_ += {
		Pad::GetLStick().x * velocity_,
		Pad::GetLStick().y * velocity_ * kY_MOVE_,
		0 };

	// 移動量の加算
	position_ += move_;
	/*rotation += rev;*/

	// 範囲制限
	position_.x = max(position_.x, -kMOVE_LIMIT_.x);
	position_.y = max(position_.y, -kMOVE_LIMIT_.y);
	position_.x = min(position_.x, +kMOVE_LIMIT_.x);
	position_.y = min(position_.y, +kMOVE_LIMIT_.y);

	// 範囲制限
	rotation_.x = max(rotation_.x, -kREV_LIMIT_.x);
	rotation_.z = max(rotation_.z, -kREV_LIMIT_.z);
	rotation_.x = min(rotation_.x, +kREV_LIMIT_.x);
	rotation_.z = min(rotation_.z, +kREV_LIMIT_.z);
}

void Player::Shot()
{
	// スペースキー or PadのAボタン のトリガー入力を受け付けた場合
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		shotTimeData_.Start(SHOT_INTERVAL_);
	}

	// スペースキー or PadのAボタン の押下入力を受け付けた場合
	if (Key::Down(DIK_SPACE) || Pad::Down(Button::A))
	{
		// ショットのタイマーがインターバルと等しくなったときにもう一度タイマーを動かす (押下しているので)
		if ((shotTimeData_.GetTime()) == SHOT_INTERVAL_)
		{
			shotTimeData_.Start(SHOT_INTERVAL_);
		}

		if (shotTimeData_.GetTime() == SHOT_DELAY_)
		{
			// 自機弾の毎フレーム移動
			Vector3 velocity = { 0, 0, 0 };
			velocity = reticleObj_.GetWorldPosition() - Object3D::GetWorldPosition();
			velocity.Normalize();
			velocity *= kBULLET_SPEED_;

			// 自機弾の自機からみたローカル発射位置
			Vector3 delayPos = { 0, 0.2f, 7.1f };

			// 速度ベクトルを自機の向きに合わせて回転
			velocity = Matrix4::Transform(velocity, matWorld_);
			//velocity = velocity * matWorld;
			delayPos = delayPos * matWorld_;
	
			// 自機弾を生成、初期化
			bullets_.push_back(std::move(make_unique<PlayerBullet>()));


			bullets_.back()->Initialize(state_.ExpandNum(), &bulletModel_, GetWorldPosition() + delayPos, velocity);
		}
	}
	// タイマーのアップデート
	shotTimeData_.Update();
}