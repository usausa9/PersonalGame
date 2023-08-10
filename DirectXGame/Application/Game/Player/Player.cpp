#include "Player.h"
#include "SphereCollider.h"
#include "WinAPI.h"
#include "CollisionAttribute.h"

using namespace Input;

// 初期化
void Player::Initialize(Camera* camera)
{
	// 自機モデル読み込み
	playerModel = OBJModel::LoadFromOBJ("vicviper");
	reticleModel = OBJModel::LoadFromOBJ("ICO");

	// レティクルスプライト割り当て
	reticleTex = TextureManager::Load(L"Resources/Sprites/reticle.png");
	reticleSp = make_unique<Sprite>(reticleTex);
	reticleSp->position = reticlePos;

	// 当たり判定デバッグ用スプライト
	aTex = TextureManager::Load(L"Resources/Sprites/texture.png");
	aSp = make_unique<Sprite>(aTex);
	aSp->position = {100,100};
	
	// 自機の行列初期化
	rotation = { 0, 0, 0 };
	position = { 0, 0, 22 };
	InitializeObject3D();

	// 自機モデルと自機オブジェクトを紐づけ
	objModel = &playerModel;
	reticleObj.objModel = &reticleModel;
	reticleObj.position = { 0, 0, 50 };
	reticleObj.InitializeObject3D();

	// プレイヤー状態の初期化
	state.Initialize();

	// コライダーの追加
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(Vector3({ 0, radius, 0 }),radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	// 自機弾モデル読み込み
	bulletModel = OBJModel::LoadFromOBJ("ICO");
}

// 更新
void Player::Update()
{
	// プレイヤー状態の更新
	state.Update();

	// 消滅フラグが立った弾を削除
	bullets.remove_if([](unique_ptr<PlayerBullet>& bullet)
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
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		bullet->Update();
	}

}

// 描画
void Player::Draw()
{
	// オブジェ描画
	DrawObject3D();
	reticleObj.DrawObject3D();

	// 弾描画
	for (unique_ptr<PlayerBullet>& bullet : bullets)
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
	matViewPort.m[0][0] =   WinAPI::GetInstance()->width  / 2.0f;
	matViewPort.m[1][1] = -(WinAPI::GetInstance()->height / 2.0f);
	matViewPort.m[3][0] =   WinAPI::GetInstance()->width  / 2.0f;
	matViewPort.m[3][1] =   WinAPI::GetInstance()->height / 2.0f;

	// カメラ行列との合成
	Matrix4 matViewProjectionViewPort = 
		Camera::GetCurrentCamera()->GetViewProjection() * matViewPort;
	
	// 画面上のレティクル座標を動かす
	Vector2 reticleMoveVel = { 0, 0 };

	// 自機の速さとレティクルのスピード調整
	reticleSpd = kReticleSpd * velocity;

	if (Key::Down(DIK_A) && Key::Down(DIK_D))
	{

	}
	else if (Key::Down(DIK_A))
	{
		reticleMoveVel.x = -reticleSpd;
	}
	else if (Key::Down(DIK_D))
	{
		reticleMoveVel.x = reticleSpd;
	}

	if (Key::Down(DIK_W) && Key::Down(DIK_S))
	{

	}
	else if (Key::Down(DIK_W))
	{
		reticleMoveVel.y = -reticleSpd * kYMoveReticle;
	}
	else if (Key::Down(DIK_S))
	{
		reticleMoveVel.y = reticleSpd * kYMoveReticle;
	}

	reticlePos += reticleMoveVel;

	// レティクル座標の移動制限
	Vector2 reticlePosMin = { reticleRadius, reticleRadius * 0.6f };
	Vector2 reticlePosMax = { WinAPI::GetInstance()->width  - reticleRadius,
							  WinAPI::GetInstance()->height - reticleRadius * 0.6f };
	
	reticlePos.x = max(reticlePos.x, reticlePosMin.x);
	reticlePos.y = max(reticlePos.y, reticlePosMin.y);
	reticlePos.x = min(reticlePos.x, reticlePosMax.x);
	reticlePos.y = min(reticlePos.y, reticlePosMax.y);

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

	// レイの方向
	Vector3 direction = posFar - posNear;
	direction.Normalize();

	// カメラからレティクルの距離
	reticleObj.position = posNear + direction * distanceReticle;

	reticleObj.UpdateObject3D();
	reticleSp->Update();
}

void Player::DrawUI()
{
	reticleSp->Draw();
	
	// 判定時描画
	for (unique_ptr<PlayerBullet>& bullet : bullets)
	{
		if (bullet->IsDrawSP())
		{
			aSp->Draw();
		};
	}

	// 自機状態の描画
	state.DrawUI();
}

// 入力受け付け + 移動
void Player::Move()
{
	// 移動量の初期化
	move = { 0,0,0 };

	// 強化に応じてスピード変化
	velocity = formerlySpeed + speedUpRate * state.SpeedUpNum();

	// WASD入力での移動
	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * velocity * kYMove,
		0 };

	/*rev = {
		(Key::Down(DIK_S) - Key::Down(DIK_W)) * rotateRev,
		0,
		((Key::Down(DIK_A) - Key::Down(DIK_D)) * rotateRev) * kYRotate };*/

	// GamePadでの移動
	move += {
		Pad::GetLStick().x * velocity,
		Pad::GetLStick().y * velocity * kYMove,
		0 };

	// 移動量の加算
	position += move;
	/*rotation += rev;*/

	// 範囲制限
	position.x = max(position.x, -kMoveLimit.x);
	position.y = max(position.y, -kMoveLimit.y);
	position.x = min(position.x, +kMoveLimit.x);
	position.y = min(position.y, +kMoveLimit.y);

	// 範囲制限
	rotation.x = max(rotation.x, -kRevLimit.x);
	rotation.z = max(rotation.z, -kRevLimit.z);
	rotation.x = min(rotation.x, +kRevLimit.x);
	rotation.z = min(rotation.z, +kRevLimit.z);
}

void Player::Shot()
{
	// スペースキー or PadのAボタン のトリガー入力を受け付けた場合
	if (Key::Trigger(DIK_SPACE) || Pad::Trigger(Button::A))
	{
		shotTimeData.Start(shotInterval);
	}

	// スペースキー or PadのAボタン の押下入力を受け付けた場合
	if (Key::Down(DIK_SPACE) || Pad::Down(Button::A))
	{
		// ショットのタイマーがインターバルと等しくなったときにもう一度タイマーを動かす (押下しているので)
		if ((shotTimeData.GetTime()) == shotInterval)
		{
			shotTimeData.Start(shotInterval);
		}

		if (shotTimeData.GetTime() == shotDelay)
		{
			// 自機弾の毎フレーム移動
			Vector3 velocity = { 0, 0, 0 };
			velocity = reticleObj.GetWorldPosition() - Object3D::GetWorldPosition();
			velocity.Normalize();
			velocity *= kBulletSpeed;

			// 自機弾の自機からみたローカル発射位置
			Vector3 delayPos = { 0, 0.2f, 7.1f };

			// 速度ベクトルを自機の向きに合わせて回転
			velocity = Matrix4::Transform(velocity, matWorld);
			//velocity = velocity * matWorld;
			delayPos = delayPos * matWorld;
	
			// 自機弾を生成、初期化
			bullets.push_back(std::move(make_unique<PlayerBullet>()));
			bullets.back()->Initialize(&bulletModel, GetWorldPosition() + delayPos, velocity);
		}
	}
	// Timerのアップデート
	shotTimeData.Update();
}