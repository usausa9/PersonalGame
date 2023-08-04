#pragma once
#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "Object3D.h"

#include "Camera.h"

#include "OBJModel.h"

#include "Skydome.h"

class CollisionManager;

class TitleScene
{
private:
	// �Փ˃}�l�[�W��
	CollisionManager* collisionManager = nullptr;

	// �J����
	Camera* camera = nullptr;

	// �v���C���[,�G
	unique_ptr<Player> player = nullptr;
	unique_ptr<Enemy> enemy = nullptr;

	// �V��
	unique_ptr<Skydome> skydome = nullptr;

public:
	// ������
	void Initialize();

	// �I������
	void Finalize();

	// ���t���[���X�V
	void Update();

	// 3D�`��
	void Draw3D();

	// �p�[�e�B�N���`��
	void DrawParticle();

	// 2D�`��
	void Draw2D();
};