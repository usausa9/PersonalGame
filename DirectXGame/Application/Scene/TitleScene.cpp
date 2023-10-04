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

	pressKeyTex_ = TextureManager::Load(L"Resources/Sprites/press_key.png");
	pressKeySprite_ = make_unique<Sprite>(pressKeyTex_);
	pressKeySprite_->position_ = PRESS_KEY_BASE_POS_;

	nowActiveTimer_ = 0;
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

	// pressKEY�̃A�j���[�V�����^�C�}�[�X�V
	if (nowActiveTimer_ == 0 && !pressAnimeTimer_[1].GetActive())
	{
		pressAnimeTimer_[0].Start(PRESS_ANIME_MAX_TIMER_);
		nowActiveTimer_ = 1;
	}
	else if (nowActiveTimer_ == 1 && !pressAnimeTimer_[0].GetActive())
	{
		pressAnimeTimer_[1].Start(PRESS_ANIME_MAX_TIMER_);
		nowActiveTimer_ = 0;
	}

	pressAnimeTimer_[0].Update();
	pressAnimeTimer_[1].Update();

	if (nowActiveTimer_ == 1)
	{
		kPressKeySpriteSize_ = {
		0.8f + (Easing::In(pressAnimeTimer_[0].GetTimeRate()) / 3),
		0.8f + (Easing::In(pressAnimeTimer_[0].GetTimeRate()) / 3)};
	}
	else
	{
		kPressKeySpriteSize_ = {
		0.8f + (Easing::In(pressAnimeTimer_[1].GetDisTimeRate()) / 3),
		0.8f + (Easing::In(pressAnimeTimer_[1].GetDisTimeRate()) / 3)};
	}

	pressKeySprite_->scale_ = { kPressKeySpriteSize_ };

	// �^�C�g���X�v���C�g�X�V
	titleSprite_->Update();
	titleSceneSprite_->Update();
	pressKeySprite_->Update();
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
	pressKeySprite_->Draw();
}

void TitleScene::PossibleStartGame()
{
	if (isStart_ == true)
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}