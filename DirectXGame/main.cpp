#include "MyGame.h"
#include "UsaFramework.h"

// Windows�A�v���ł̃G���g���[�|�C���g (main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �Q�[���C���X�^���X����
	UsaFramework* game = new MyGame;

	game->Run();

	delete game;

	return 0;
}