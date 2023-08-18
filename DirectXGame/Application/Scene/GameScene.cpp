#include "GameScene.h"
#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void GameScene::Initialize()
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

	// �v���C���[������
	player_ = make_unique<Player>();
	player_.get()->Initialize(camera_);

	// �V��������
	skydome_ = make_unique<Skydome>();
	skydome_.get()->Initialize();
}

void GameScene::Finalize()
{
	delete railCamera_;
}

void GameScene::Update()
{
	// DirectX���t���[������(�X�V����) ��������
	railCamera_->Update();

	// �v���C���[�̍X�V
	player_->SetParent(railCamera_->GetObject3d());
	player_->Update();

	// �G�l�~�[�̍X�V
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update(railCamera_->GetObject3d()->matWorld_);
	}

	// ����ł�G������
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)
	{
		if (!enemy->IsAlive()) 
		{
			return true;
		}

		return false; 
	});

	if (Key::Trigger(DIK_Q)) 
	{
		EnemySpawn();
	}

	// �V���̍s��X�V
	skydome_->Update();
	
	// �J���������[���J�����̂��̂�
	camera_ = railCamera_->GetCamera();
	camera_->Update();

	// �S�Ă̏Փ˂��`�F�b�N (�X�V�̍Ō�)
	collisionManager_->CheckAllCollisions();
}

void GameScene::Draw3D()
{
	// �J�����Z�b�g
	camera_->Set();

	// �V���`��
	skydome_->Draw();

	// �v���C���[�`��
	player_->Draw();

	// �G�`��
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void GameScene::DrawParticle()
{
	// �J�����Z�b�g
	camera_->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void GameScene::Draw2D()
{
	player_->DrawUI();
}

void GameScene::EnemySpawn()
{
	// �X�v���C������_
	float z = 40.0f;
	Vector3 start { 0,0,z };
	Vector3 p1 =  { 5,5,z };
	Vector3 p2 =  { -5,-5,z };
	Vector3 p3 =  { 5,5,z };
	Vector3 end = { -10,-10,z };

	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,end };

	// �G�̐����Ə�����
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->Spawn();

	// ���X�g�ɓo�^
	enemys_.push_back(std::move(newEnemy));
}