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

class CollisionManager;

class TitleScene : public IScene
{
private:
	// �Փ˃}�l�[�W��
	CollisionManager* collisionManager = nullptr;

	// �J����
	Camera* camera = nullptr;
	RailCamera* railCamera = nullptr;

	// �v���C���[,�G
	std::list<std::unique_ptr<Enemy>> enemys;

	// �V��
	unique_ptr<Skydome> skydome = nullptr;

	// �V�[���؂�ւ��t���O
	bool isStart = false;

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
};