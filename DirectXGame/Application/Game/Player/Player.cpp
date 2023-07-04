#include "Player.h"

using namespace Input;

void Player::Initialize()
{
	playerModel = OBJModel::LoadFromOBJ("vicviper");
}

void Player::Update(Object3D player)
{
	player.position += Move();
}

Vector3 Player::Move()
{
	move = { 0,0,0 };

	if (Key::Down(DIK_D) )
	{
		move += { Velocity, 0, 0 };
	}
	else if (Key::Down(DIK_A))
	{
		move += { -Velocity, 0, 0 };
	}

	if (Key::Down(DIK_W))
	{
		move += { 0, Velocity, 0 };
	}
	else if (Key::Down(DIK_S))
	{
		move += { 0, -Velocity, 0 };
	}

	return move;
}