/**
 * @file	main.cpp
 * @brief	Windowsアプリでのエントリーポイント
**/

#include "MyGame.h"
#include "UsaFramework.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ゲームインスタンス生成
	UsaFramework* game = new MyGame;

	game->Run();

	delete game;

	return 0;
}