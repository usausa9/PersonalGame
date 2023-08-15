#pragma once
#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "PowerUpItem.h"

#include "Object3D.h"
#include "FBXObject3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"
#include "FBXModel.h"

#include "Skydome.h"

#include "FbxLoader.h"

class CollisionManager;

class GameScene
{
private:
	// �Փ˃}�l�[�W��
	CollisionManager* collisionManager = nullptr;
	
	// �J����
	Camera* camera = nullptr;
	RailCamera* railCamera = nullptr;

	// �v���C���[,�G
	unique_ptr<Player> player = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys;

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

	// �G
	void EnemySpawn();
};