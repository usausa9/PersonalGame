#include "GameScene.h"
#include "CollisionManager.h"

#include "ImGuiManager.h"

using namespace Input;

void GameScene::Initialize()
{
	// �����蔻��
	collisionManager = CollisionManager::GetInstance();

	// �p�[�e�B�N���p�̃p�C�v���C���EInit
	ParticleManager::CreatePipeline();

	// �J����������
	camera = new Camera;
	camera->Initialize();

	// ���[���J����������
	railCamera = new RailCamera();
	railCamera->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

	// �v���C���[������
	player = make_unique<Player>();
	player.get()->Initialize(camera);

	// �V��������
	skydome = make_unique<Skydome>();
	skydome.get()->Initialize();
}

void GameScene::Finalize()
{
	delete railCamera;
}

void GameScene::Update()
{
	// DIrectX���t���[������(�X�V����) ��������
	railCamera->Update();

	// �v���C���[�̍X�V
	player->SetParent(railCamera->GetObject3d());
	player->Update();

	// �G�l�~�[�̍X�V
	for (std::unique_ptr<Enemy>& enemy : enemys) 
	{
		enemy->Update(railCamera->GetObject3d()->matWorld, player->GetWorldPosition());
	}

	// ����ł�G������
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) 
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
	skydome->Update();
	
	// �J���������[���J�����̂��̂�
	camera = railCamera->GetCamera();
	camera->Update();

	// �S�Ă̏Փ˂��`�F�b�N (�X�V�̍Ō�)
	collisionManager->CheckAllCollisions();
}

void GameScene::Draw3D()
{
	// �J�����Z�b�g
	camera->Set();

	// �V���`��
	skydome->Draw();

	// �v���C���[�`��
	player->Draw();

	// �G�`��
	for (std::unique_ptr<Enemy>& enemy : enemys) 
	{
		enemy->Draw();
	}
}

void GameScene::DrawParticle()
{
	// �J�����Z�b�g
	camera->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void GameScene::Draw2D()
{
	player->DrawUI();
}

void GameScene::EnemySpawn()
{
	float z = 40.0f;

	Vector3 start { -10,0,z };
	Vector3 p1 =  { 0,5,z };
	Vector3 p2 =  { -5,0,z };
	Vector3 p3 =  { 5,0,z };
	Vector3 end = { 10,0,z };

	std::vector<Vector3> enemyMovePoints = { start,p1,p2,p3,end };

	// �G�̐����Ə�����
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints);
	newEnemy->Spawn();

	// ���X�g�ɓo�^
	enemys.push_back(std::move(newEnemy));
}