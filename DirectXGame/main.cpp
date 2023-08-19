#include "MyGame.h"
#include "UsaFramework.h"

// Windowsアプリでのエントリーポイント (main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ゲームインスタンス生成
	UsaFramework* game = new MyGame;

	game->Run();

	delete game;

	return 0;
}