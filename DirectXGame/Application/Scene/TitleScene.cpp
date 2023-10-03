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

	// �V��
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();

	// �V���̍s��X�V
	skydome_->Update();

	// �^�C�g���X�v���C�g
	titleTex_ = TextureManager::Load(L"Resources/Sprites/title.png");
	titleSprite_ = make_unique<Sprite>(titleTex_);
	titleSprite_->position_ = TITLE_BASE_POS_;

	titleSceneTex_ = TextureManager::Load(L"Resources/Sprites/title_scene.png");
	titleSceneSprite_ = make_unique<Sprite>(titleSceneTex_);
	titleSceneSprite_->position_ = TITLE_SCENE_BASE_POS_;

	plessKeyTex_ = TextureManager::Load(L"Resources/Sprites/pless_key.png");
	plessKeySprite_ = make_unique<Sprite>(plessKeyTex_);
	plessKeySprite_->position_ = PLESS_KEY_BASE_POS_;

	plessAnimeTimer.Start(float)
}

void TitleScene::Finalize()
{
	delete railCamera_;
}

void TitleScene::Update()
{
	// SPACE�L�[����������Q�[���V�[���ɁB
	isStart_ = false;

	if (Key::Trigger(DIK_SPACE))
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

	// �^�C�g���X�v���C�g�X�V
	titleSprite_->Update();
	titleSceneSprite_->Update();
	plessKeySprite_->Update();
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
}

void TitleScene::Draw2D()
{
	//titleSprite_->Draw();
	titleSceneSprite_->Draw();
	plessKeySprite_->Draw();
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}