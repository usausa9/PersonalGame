#include "TitleScene.h"
#include "SceneManager.h"

#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
{
	// �p�[�e�B�N���p�̃p�C�v���C���EInit
	ParticleManager::CreatePipeline();

	// �J����������
	camera_ = new Camera;
	camera_->Initialize();

	// ���[���J����������
	railCamera_ = new RailCamera();
	railCamera_->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// �^�C�g���X�v���C�g
	titleTex_ = TextureManager::Load(L"Resources/Sprites/title.png");
	titleSprite_ = make_unique<Sprite>(titleTex_);
	titleSprite_->position_ = SPRITE_BASE_POS_;
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

	// �J���������[���J�����̂��̂�
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// �^�C�g���X�v���C�g�X�V3
	titleSprite_->Update();
}

void TitleScene::Draw3D()
{
	// �J�����Z�b�g
	camera_->Set();
}

void TitleScene::DrawParticle()
{
	// �J�����Z�b�g
	camera_->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void TitleScene::Draw2D()
{
	titleSprite_->Draw();
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}