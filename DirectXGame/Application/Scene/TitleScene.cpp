#include "TitleScene.h"
#include "SceneManager.h"

#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
{
	// �����蔻��
	collisionManager_ = CollisionManager::GetInstance();

	// �p�[�e�B�N���p�̃p�C�v���C���EInit
	ParticleManager::CreatePipeline();

	// �J����������
	camera_ = new Camera;
	camera_->Initialize();

	// ���[���J����������
	railCamera_ = new RailCamera();
	railCamera_->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// �V��������
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();
}

void TitleScene::Finalize()
{
	delete railCamera_;
}

void TitleScene::Update()
{
	// ��肠��������T�L�[����������Q�[���V�[���ɁB
	isStart_ = false;

	if (Key::Trigger(DIK_T))
	{
		isStart_ = true;
	}

	// �X�^�[�g�t���O����������Q�[���V�[����
	PossibleStartGame();

	// DirectX���t���[������(�X�V����) ��������
	railCamera_->Update();

	// �V���̍s��X�V
	skydome_->Update();

	// �J���������[���J�����̂��̂�
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// �S�Ă̏Փ˂��`�F�b�N (�X�V�̍Ō�)
	collisionManager_->CheckAllCollisions();
}

void TitleScene::Draw3D()
{
	// �J�����Z�b�g
	camera_->Set();

	// �V���`��
	skydome_->Draw();
}

void TitleScene::DrawParticle()
{
	// �J�����Z�b�g
	camera_->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void TitleScene::Draw2D()
{
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}