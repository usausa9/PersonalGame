#include "PlayerState.h"

using namespace Input;

void PlayerState::SetTexture()
{
	// レティクルスプライト割り当て
	stateSelectTex[0]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName1a.png");
	stateSelectTex[1]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName1b.png");
	stateSelectTex[2]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName2a.png");
	stateSelectTex[3]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName2b.png");
	stateSelectTex[4]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName3a.png");
	stateSelectTex[5]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName3b.png");
	stateSelectTex[6]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName4a.png");
	stateSelectTex[7]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName4b.png");
	stateSelectTex[8]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName5a.png");
	stateSelectTex[9]  = TextureManager::Load(L"Resources/Sprites/StateName/StateName5b.png");
	
	stateSelectTex[10] = TextureManager::Load(L"Resources/Sprites/StateName/StateName6a.png");
	stateSelectTex[11] = TextureManager::Load(L"Resources/Sprites/StateName/StateName6b.png");
	
	// 0-9枚目までスプライトに紐づけ
	for (int i = 0; i < spriteNum / 2; i++)
	{
		stateSprite[i] = make_unique<Sprite>(stateSelectTex[i]);
 	}

	// 10-14枚目までスプライトに紐づけ
	for (int i = spriteNum / 2; i < spriteNum - powerUpNum; i++)
	{
		stateSprite[i] = make_unique<Sprite>(stateSelectTex[10]);
	}

	// 14-19枚目までスプライトに紐づけ
	for (int i = (spriteNum / 2) + powerUpNum; i < spriteNum ; i++)
	{
		stateSprite[i] = make_unique<Sprite>(stateSelectTex[11]);
	}

	// サイズ設定
	for (int i = 0; i < spriteNum; i++)
	{
		stateSprite[i]->scale = { scaleTexUI, scaleTexUI };
	}

	// 位置移動
	stateSprite[0] ->position = spriteBase;
	stateSprite[1] ->position = spriteBase;
	stateSprite[10]->position = spriteBase;
	stateSprite[15]->position = spriteBase;

	stateSprite[2]-> position = spriteBase + Vector2(spriteSize, 0);
	stateSprite[3]-> position = spriteBase + Vector2(spriteSize, 0);
	stateSprite[11]->position = spriteBase + Vector2(spriteSize, 0);
	stateSprite[16]->position = spriteBase + Vector2(spriteSize, 0);

	stateSprite[4] ->position = spriteBase + Vector2(spriteSize * 2, 0);
	stateSprite[5] ->position = spriteBase + Vector2(spriteSize * 2, 0);
	stateSprite[12]->position = spriteBase + Vector2(spriteSize * 2, 0);
	stateSprite[17]->position = spriteBase + Vector2(spriteSize * 2, 0);

	stateSprite[6]-> position = spriteBase + Vector2(spriteSize * 3, 0);
	stateSprite[7]-> position = spriteBase + Vector2(spriteSize * 3, 0);
	stateSprite[13]->position = spriteBase + Vector2(spriteSize * 3, 0);
	stateSprite[18]->position = spriteBase + Vector2(spriteSize * 3, 0);

	stateSprite[8]-> position = spriteBase + Vector2(spriteSize * 4, 0);
	stateSprite[9]-> position = spriteBase + Vector2(spriteSize * 4, 0);
	stateSprite[14]->position = spriteBase + Vector2(spriteSize * 4, 0);
	stateSprite[19]->position = spriteBase + Vector2(spriteSize * 4, 0);
}

void PlayerState::Initialize()
{	
	// 画像紐づけ
	SetTexture();

	for (int i = 0; i < uint8_t(State::BARRIER); i++)
	{
		powerUpSelect[i] = uint8_t(SelectState::NONE);
		powerUpStatus[i] = uint8_t(SelectState::NONE);
	}

	nowSelect = uint8_t(State::_NONE);

	isPowerUpAble = false;
}

void PlayerState::Update()
{	
	if (nowSelect == uint8_t(State::SPEEDUP))
	{
		if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USED))
		{
			powerUpSelect[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::USEDSELECT);
		}
	}
	else if (nowSelect == uint8_t(State::EXPAND))
	{
		if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect[uint8_t(State::EXPAND)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::USED))
		{
			powerUpSelect[uint8_t(State::EXPAND)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::USED);
		}
	}
	else if (nowSelect == uint8_t(State::LASER))
	{
		if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect[uint8_t(State::LASER)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::USED))
		{
			powerUpSelect[uint8_t(State::LASER)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect[uint8_t(State::EXPAND)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect[uint8_t(State::EXPAND)] = uint8_t(SelectState::USED);
		}
	}
	else if (nowSelect == uint8_t(State::OPTION))
	{
		if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect[uint8_t(State::OPTION)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::USED))
		{
			powerUpSelect[uint8_t(State::OPTION)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect[uint8_t(State::LASER)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect[uint8_t(State::LASER)] = uint8_t(SelectState::USED);
		}
	}
	else if (nowSelect == uint8_t(State::BARRIER))
	{
		if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::NONE))
		{
			powerUpSelect[uint8_t(State::BARRIER)] = uint8_t(SelectState::SELECT);
		}
		else if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::USED))
		{
			powerUpSelect[uint8_t(State::BARRIER)] = uint8_t(SelectState::USEDSELECT);
		}

		if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect[uint8_t(State::OPTION)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect[uint8_t(State::OPTION)] = uint8_t(SelectState::USED);
		}
	}
	else 
	{
		if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::SELECT))
		{
			powerUpSelect[uint8_t(State::BARRIER)] = uint8_t(SelectState::NONE);
		}
		else if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::USEDSELECT))
		{
			powerUpSelect[uint8_t(State::BARRIER)] = uint8_t(SelectState::USED);
		}
	}

	if (Key::Trigger(DIK_9))
	{
		if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus[0]++;
			nowSelect = uint8_t(State::_NONE);

			if (powerUpStatus[0] == maxSpeedUpNum)
			{
				powerUpSelect[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect[uint8_t(State::SPEEDUP)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus[1]++;
			nowSelect = uint8_t(State::_NONE);
			
			if (powerUpStatus[1] == maxExpandNum)
			{
				powerUpSelect[uint8_t(State::EXPAND)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect[uint8_t(State::EXPAND)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus[2]++;
			nowSelect = uint8_t(State::_NONE);

			if (powerUpStatus[2] == maxLaserNum)
			{
				powerUpSelect[uint8_t(State::LASER)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect[uint8_t(State::LASER)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus[3]++;
			nowSelect = uint8_t(State::_NONE);

			if (powerUpStatus[3] == maxOptionNum)
			{
				powerUpSelect[uint8_t(State::OPTION)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect[uint8_t(State::OPTION)] = uint8_t(SelectState::NONE);
			}
		}

		if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::SELECT))
		{
			powerUpStatus[4]++;
			nowSelect = uint8_t(State::_NONE);

			if (powerUpStatus[4] == maxBarrierNum)
			{
				powerUpSelect[uint8_t(State::BARRIER)] = uint8_t(SelectState::USED);
			}
			else
			{
				powerUpSelect[uint8_t(State::BARRIER)] = uint8_t(SelectState::NONE);
			}
		}
	}

	if (Key::Trigger(DIK_0))
	{
		if (nowSelect == uint8_t(State::_NONE))
		{
			nowSelect = uint8_t(State::SPEEDUP);
		}
		else
		{
			nowSelect++;
		}
	}

	if (Key::Trigger(DIK_M))
	{
		isPowerUpAble = true;
	}

	// サイズ設定
	for (int i = 0; i < spriteNum; i++)
	{
		stateSprite[i]->Update();
	}
}

void PlayerState::DrawUI()
{
	// SPEEDUP 描画
	if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::NONE))
	{
		stateSprite[0]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::SELECT))
	{
		stateSprite[1]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USED))
	{
		stateSprite[10]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::SPEEDUP)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite[15]->Draw();
	}

	// EXPAND 描画
	if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::NONE))
	{
		stateSprite[2]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::SELECT))
	{
		stateSprite[3]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::USED))
	{
		stateSprite[11]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::EXPAND)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite[16]->Draw();
	}

	// LASER 描画
	if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::NONE))
	{
		stateSprite[4]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::SELECT))
	{
		stateSprite[5]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::USED))
	{
		stateSprite[12]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::LASER)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite[17]->Draw();
	}

	// OPTION 描画
	if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::NONE))
	{
		stateSprite[6]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::SELECT))
	{
		stateSprite[7]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::USED))
	{
		stateSprite[13]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::OPTION)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite[18]->Draw();
	}

	// BARRIER 描画
	if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::NONE))
	{
		stateSprite[8]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::SELECT))
	{
		stateSprite[9]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::USED))
	{
		stateSprite[14]->Draw();
	}
	else if (powerUpSelect[uint8_t(State::BARRIER)] == uint8_t(SelectState::USEDSELECT))
	{
		stateSprite[19]->Draw();
	}
}
