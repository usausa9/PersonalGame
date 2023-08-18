#include "MyGame.h"
#include "UsaFramework.h"
#include "SceneFactory.h"

void MyGame::Initialize()
{
	// ���N���X�̏���������
	UsaFramework::Initialize();

	// �V�[���t�@�N�g���[�𐶐����}�l�[�W���ɃZ�b�g
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	// ���N���X�̏I������
	UsaFramework::Finalize();
}

void MyGame::Update()
{
	// ���N���X�̍X�V����
	UsaFramework::Update();
}

void MyGame::Draw()
{
	// ���N���X�̕`�揈��
	UsaFramework::Draw();
}