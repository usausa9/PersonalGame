#include "MyGame.h"
#include "UsaFramework.h"

// Windows�A�v���ł̃G���g���[�|�C���g (main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �Q�[���C���X�^���X����
	//MyGame game;
	UsaFramework* game = new MyGame;

	game->Run();

	delete game;

	//// �Q�[���̏�����
	//game.Initialize();

	//// �Q�[�����[�v
	//while (true)
	//{
	//	// �X�V����
	//	game.Update();

	//	// �I���t���O�Ńu���C�N
	//	if (game.IsEndRequest() == true)
	//	{
	//		break;
	//	}

	//	// �`�揈��
	//	game.Draw();
	//}

	//// �Q�[���̏I��
	//game.Finalize();

	return 0;
}