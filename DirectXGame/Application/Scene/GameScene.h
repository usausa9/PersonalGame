#pragma once
#include "IScene.h"

#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"
#include "Enemy.h"

#include "Object3D.h"
#include "FBXObject3D.h"

#include "Camera.h"
#include "RailCamera.h"

#include "OBJModel.h"

#include "Skydome.h"

#include <iostream>
#include <string>
#include <sstream>

class CollisionManager;

enum class Phase : uint8_t
{
	IDLE = 0x0000,
	MOB = 0x0001,
	BOSS = 0x0002,
};

class GameScene : public IScene
{
private:
	// �Փ˃}�l�[�W��
	CollisionManager* collisionManager_ = nullptr;
	
	// �J����
	Camera* camera_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	// �v���C���[,�G
	unique_ptr<Player> player_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;

	// �G�@���f��
	OBJModel enemyModel_[2];

	// �V��
	unique_ptr<Skydome> skydome_ = nullptr;

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

public:
	// �G
	void EnemySpawn(uint8_t enemyKind, uint8_t trajectoryKind);

	// �t�@�C���ǂݍ���
	void LoadCsvFile();

	// �ǂݍ��񂾃f�[�^����G�̎�ޕt����
	void UpdateEnemyData();

	// �G�̋N�����
	std::vector<Vector3> TrajectoryKind(uint8_t trajectoryKind);

	// �G�����R�}���h
	std::stringstream enemyData_;

	// �G�����҂�����
	TimeData waitTimer_;

	// �G�҂��t���O
	bool isStandBy_ = false;
};