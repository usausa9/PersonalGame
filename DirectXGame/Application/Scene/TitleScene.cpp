#include "TitleScene.h"
#include "SceneManager.h"

#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
{
	// �����蔻��
	collisionManager = CollisionManager::GetInstance();

	// �p�[�e�B�N���p�̃p�C�v���C���EInit
	ParticleManager::CreatePipeline();

	// �J����������
	camera = new Camera;
	camera->Initialize();

	// ���[���J����������
	railCamera = new RailCamera();
	railCamera->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// �V��������
	skydome = make_unique<Skydome>();
	skydome.get()->Initialize();
}

void TitleScene::Finalize()
{
	delete railCamera;
}

void TitleScene::Update()
{
	// ��肠��������T�L�[����������Q�[���V�[���ɁB
	isStart = false;

	if (Key::Trigger(DIK_T))
	{
		isStart = true;
	}

	// �X�^�[�g�t���O����������Q�[���V�[����
	PossibleStartGame();

	// DirectX���t���[������(�X�V����) ��������
	railCamera->Update();

	// �V���̍s��X�V
	skydome->Update();

	// �J���������[���J�����̂��̂�
	camera = railCamera->GetCamera();
	camera->Update();

	// �S�Ă̏Փ˂��`�F�b�N (�X�V�̍Ō�)
	collisionManager->CheckAllCollisions();
}

void TitleScene::Draw3D()
{
	// �J�����Z�b�g
	camera->Set();

	// �V���`��
	skydome->Draw();
}

void TitleScene::DrawParticle()
{
	// �J�����Z�b�g
	camera->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void TitleScene::Draw2D()
{
}

void TitleScene::PossibleStartGame()
{
	if (isStart == true)
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}