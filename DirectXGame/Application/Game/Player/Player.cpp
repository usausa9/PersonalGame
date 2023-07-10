#include "Player.h"

using namespace Input;

// ‰Šú‰»
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

// XV
void Player::Update()
{
	// “ü—Í‚©‚ç‚ÌˆÚ“®ˆ—
	Move();

	// ’e”­ŽËˆ— / XV
	Shot();
	if (bullet) 
	{
		bullet->Update();
	}

	// s—ñXV •K‚¸ŒÄ‚Ño‚·
	playerObj.UpdateObject3D();
}

// •`‰æ
void Player::Draw()
{
	// ƒIƒuƒWƒF•`‰æ
	playerObj.DrawObject3D();

	// ’e•`‰æ
	if (bullet)
	{
		bullet->Draw();
	}
}

// “ü—ÍŽó‚¯•t‚¯ + ˆÚ“®
void Player::Move()
{
	move = { 0,0,0 };

	move += { 
		(Key::Down(DIK_D) - Key::Down(DIK_A)) * Velocity, 
		(Key::Down(DIK_W) - Key::Down(DIK_S)) * Velocity * kYMove,
		0 };
	
	playerObj.position += move;

	// ”ÍˆÍ§ŒÀ
	playerObj.position.x = max(playerObj.position.x, -kMoveLimit.x);
	playerObj.position.y = max(playerObj.position.y, -kMoveLimit.y);
	playerObj.position.x = min(playerObj.position.x, +kMoveLimit.x);
	playerObj.position.y = min(playerObj.position.y, +kMoveLimit.y);
}

void Player::Shot()
{
	if (Key::Trigger(DIK_SPACE))
	{
		// Ž©‹@’e‚ð¶¬A‰Šú‰»
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(&bulletModel, playerObj.position);

		// ’e‚ð“o˜^
		bullet = newBullet;
	}
}