/**
 * @file	PlayerState.cpp
 * @brief	プレイヤー強化状態のデータ保持や初期化/更新/描画を行うもの
**/

#include "PlayerState.h"
#include "Enemy.h"

using namespace Input;

void PlayerState::SetTexture()
{
	// レティクルスプライト割り当て
	stateSelectTex_[0] = TextureManager::Load(L"Resources/Sprites/StateName/StateName1a.png");
	stateSelectTex_[1] = TextureManager::Load(L"Resources/Sprites/StateName/StateName1b.png");
	stateSelectTex_[2] = TextureManager::Load(L"Resources/Sprites/StateName/StateName2a.png");
	stateSelectTex_[3] = TextureManager::Load(L"Resources/Sprites/StateName/StateName2b.png");
	stateSelectTex_[4] = TextureManager::Load(L"Resources/Sprites/StateName/StateName3a.png");
	stateSelectTex_[5] = TextureManager::Load(L"Resources/Sprites/StateName/StateName3b.png");
	stateSelectTex_[6] = TextureManager::Load(L"Resources/Sprites/StateName/StateName4a.png");
	stateSelectTex_[7] = TextureManager::Load(L"Resources/Sprites/StateName/StateName4b.png");
	stateSelectTex_[8] = TextureManager::Load(L"Resources/Sprites/StateName/StateName5a.png");
	stateSelectTex_[9] = TextureManager::Load(L"Resources/Sprites/StateName/StateName5b.png");

	stateSelectTex_[10] = TextureManager::Load(L"Resources/Sprites/StateName/StateName6a.png");
	stateSelectTex_[11] = TextureManager::Load(L"Resources/Sprites/StateName/StateName6b.png");

	// 0-9枚目までスプライトに紐づけ
	for (uint8_t i = 0; i < SPRITE_NUM_ / 2; i++)
	{
		stateSprite_[i] = make_unique<Sprite>(stateSelectTex_[i]);
	}

	// 10-14枚目までスプライトに紐づけ
	for (uint8_t i = SPRITE_NUM_ / 2; i < SPRITE_NUM_ - POWER_UP_NUM_; i++)
	{
		stateSprite_[i] = make_unique<Sprite>(stateSelectTex_[10]);
	}

	// 14-19枚目までスプライトに紐づけ
	for (uint8_t i = (SPRITE_NUM_ / 2) + POWER_UP_NUM_; i < SPRITE_NUM_; i++)
	{
		stateSprite_[i] = make_unique<Sprite>(stateSelectTex_[11]);
	}

	// サイズ設定
	for (uint8_t i = 0; i < SPRITE_NUM_; i++)
	{
		stateSprite_[i]->scale_ = { SCALE_TEX_UI_, SCALE_TEX_UI_ };
	}

	// 位置移動
	stateSprite_[0]->position_ = SPRITE_BASE_POS_;
	stateSprite_[1]->position_ = SPRITE_BASE_POS_;
	stateSprite_[10]->position_ = SPRITE_BASE_POS_;
	stateSprite_[15]->position_ = SPRITE_BASE_POS_;

	stateSprite_[2]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_, 0);
	stateSprite_[3]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_, 0);
	stateSprite_[11]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_, 0);
	stateSprite_[16]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_, 0);

	stateSprite_[4]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 2, 0);
	stateSprite_[5]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 2, 0);
	stateSprite_[12]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 2, 0);
	stateSprite_[17]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 2, 0);

	stateSprite_[6]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 3, 0);
	stateSprite_[7]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 3, 0);
	stateSprite_[13]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 3, 0);
	stateSprite_[18]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 3, 0);

	stateSprite_[8]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 4, 0);
	stateSprite_[9]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 4, 0);
	stateSprite_[14]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 4, 0);
	stateSprite_[19]->position_ = SPRITE_BASE_POS_ + Vector2(s_SPRITE_SIZE_ * 4, 0);
}

void PlayerState::Initialize()
{
	// 画像紐づけ
	SetTexture();

	for (uint8_t i = 0; i < uint8_t(State::BARRIER); i++)
	{
		powerUpSelect_[i] = uint8_t(SelectState::NONE);
		powerUpStatus_[i] = uint8_t(SelectState::NONE);
	}

	nowSelect_ = uint8_t(State::NONESTATE);
}

void PlayerState::Update()
{
	if (nowSelect_ == uint8_t(State::SPEEDUP))
	{
		if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect_[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USED))
		{
			powerUpSelect_[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::USEDSELECT);
		}
	}
	else if (nowSelect_ == uint8_t(State::EXPAND))
	{
		if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect_[uint8_t(State::EXPAND)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::USED))
		{
			powerUpSelect_[uint8_t(State::EXPAND)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect_[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect_[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::USED);
		}
	}
	else if (nowSelect_ == uint8_t(State::LASER))
	{
		if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect_[uint8_t(State::LASER)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::USED))
		{
			powerUpSelect_[uint8_t(State::LASER)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect_[uint8_t(State::EXPAND)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect_[uint8_t(State::EXPAND)] = uint8_t(SelectState::USED);
		}
	}
	else if (nowSelect_ == uint8_t(State::OPTION))
	{
		if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect_[uint8_t(State::OPTION)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::USED))
		{
			powerUpSelect_[uint8_t(State::OPTION)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect_[uint8_t(State::LASER)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect_[uint8_t(State::LASER)] = uint8_t(SelectState::USED);
		}
	}
	else if (nowSelect_ == uint8_t(State::BARRIER))
	{
		if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect_[uint8_t(State::BARRIER)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::USED))
		{
			powerUpSelect_[uint8_t(State::BARRIER)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect_[uint8_t(State::OPTION)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect_[uint8_t(State::OPTION)] = uint8_t(SelectState::USED);
		}
	}
	else
	{
		if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect_[uint8_t(State::BARRIER)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect_[uint8_t(State::BARRIER)] = uint8_t(SelectState::USED);
		}
	}

	if (Key::Trigger(DIK_0))
	{
		if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus_[0]++;
			nowSelect_ = uint8_t(State::NONESTATE);

			if (powerUpStatus_[0] == MAX_SPEEDUP_NUM_)
			{
				powerUpSelect_[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect_[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus_[1]++;
			nowSelect_ = uint8_t(State::NONESTATE);

			if (powerUpStatus_[1] == MAX_EXPAND_NUM_)
			{
				powerUpSelect_[uint8_t(State::EXPAND)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect_[uint8_t(State::EXPAND)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus_[2]++;
			nowSelect_ = uint8_t(State::NONESTATE);

			if (powerUpStatus_[2] == MAX_LASER_NUM_)
			{
				powerUpSelect_[uint8_t(State::LASER)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect_[uint8_t(State::LASER)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus_[3]++;
			nowSelect_ = uint8_t(State::NONESTATE);

			if (powerUpStatus_[3] == MAX_OPTION_NUM_)
			{
				powerUpSelect_[uint8_t(State::OPTION)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect_[uint8_t(State::OPTION)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus_[4]++;
			nowSelect_ = uint8_t(State::NONESTATE);

			if (powerUpStatus_[4] == MAX_BARRIER_NUM_)
			{
				powerUpSelect_[uint8_t(State::BARRIER)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect_[uint8_t(State::BARRIER)] = uint8_t(SelectState::NONE);
			}
		}
	}

	if (Enemy::IsOnCol() == true || Key::Trigger(DIK_9))
	{
		if (nowSelect_ == uint8_t(State::NONESTATE))
		{
			nowSelect_ = uint8_t(State::SPEEDUP);
		}
		else
		{
			nowSelect_++;
		}
	}

	// サイズ設定
	for (uint8_t i = 0; i < SPRITE_NUM_; i++)
	{
		stateSprite_[i]->Update();
	}
}

void PlayerState::DrawUI()
{
	// SPEEDUP 描画
	if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::NONE))
	{
		stateSprite_[0]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::SELECT))
	{
		stateSprite_[1]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USED))
	{
		stateSprite_[10]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite_[15]->Draw();
	}

	// EXPAND 描画
	if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::NONE))
	{
		stateSprite_[2]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::SELECT))
	{
		stateSprite_[3]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::USED))
	{
		stateSprite_[11]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::EXPAND)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite_[16]->Draw();
	}

	// LASER 描画
	if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::NONE))
	{
		stateSprite_[4]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::SELECT))
	{
		stateSprite_[5]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::USED))
	{
		stateSprite_[12]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::LASER)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite_[17]->Draw();
	}

	// OPTION 描画
	if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::NONE))
	{
		stateSprite_[6]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::SELECT))
	{
		stateSprite_[7]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::USED))
	{
		stateSprite_[13]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::OPTION)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite_[18]->Draw();
	}

	// BARRIER 描画
	if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::NONE))
	{
		stateSprite_[8]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::SELECT))
	{
		stateSprite_[9]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::USED))
	{
		stateSprite_[14]->Draw();
	}
	else if (powerUpSelect_[uint8_t(State::BARRIER)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite_[19]->Draw();
	}
}