#pragma once
#include "IScene.h"

#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "Object3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"

#include "Skydome.h"
#include "TimeData.h"

class CollisionManager;

class TitleScene : public IScene
{
public:
	// ������
	void Initialize() override;

	// �I������
	void Finalize() override;

	// ���t���[���X�V
	void Update() override;

	// 3D�`��
	void Draw3D() override;

	// �p�[�e�B�N���`��
	void DrawParticle() override;

	// 2D�`��
	void Draw2D() override;

	// �Q�[���V�[����
	void PossibleStartGame();

private:
	// �J����
	Camera* camera_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	// �v���C���[,�G
	std::list<std::unique_ptr<Enemy>> enemys_;

	// �V�[���؂�ւ��t���O
	bool isStart_ = false;

	// �^�C�g���X�v���C�g
	TextureIndex titleTex_ = {};
	TextureIndex titleSceneTex_ = {};
	TextureIndex pressKeyTex_ = {};
	unique_ptr<Sprite> titleSprite_ = nullptr;
	unique_ptr<Sprite> titleSceneSprite_ = nullptr;
	unique_ptr<Sprite> pressKeySprite_ = nullptr;
	const Vector2 TITLE_BASE_POS_ = { 640, 360 };
	const Vector2 TITLE_SCENE_BASE_POS_ = { 640, 240 };
	const Vector2 PRESS_KEY_BASE_POS_ = { 640, 480 };

	// �v���X�L�[�̑傫���̃X�J���[
	Vector2 kPressKeySpriteSize_ = {1.f, 1.f};

	// �v���X�L�[�̃A�j���[�V�����p
	TimeData pressAnimeTimer_[2];
	const float PRESS_ANIME_MAX_TIMER_ = 37.f;
	uint8_t nowActiveTimer_ = 0;

	// �V��
	unique_ptr<Skydome> skydome_ = nullptr;
};