#include "GameScene.h"
#include "CollisionManager.h"

#include "ImGuiManager.h"

#include <fstream>

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

	// �G�f�[�^�ǂݍ���
	LoadCsvFile();
}

void GameScene::Finalize()
{
	delete railCamera_;
}

void GameScene::Update()
{
	// DirectX���t���[������(�X�V����) ��������
	railCamera_->Update();

	waitTimer_.Update();

	// �G�̍X�V
	UpdateEnemyData();

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

	if (Key::Trigger(DIK_Y)) 
	{
		EnemySpawn(uint8_t(EnemyKinds::NORMAL), uint8_t(TrajectoryKinds::LEFT_2_RIGHT));
	}
	else if (Key::Trigger(DIK_U))
	{
		EnemySpawn(uint8_t(EnemyKinds::POWER), uint8_t(TrajectoryKinds::RIGHT_2_LEFT));
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

void GameScene::EnemySpawn(uint8_t enemyKind, uint8_t trajectoryKind)
{
	std::vector<Vector3> enemyMovePoints = TrajectoryKind(trajectoryKind);

	// �G�̐����Ə�����
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyMovePoints, enemyKind);
	newEnemy->Spawn();
	// ���X�g�ɓo�^
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::LoadCsvFile()
{
	// �t�@�C�����J��
	std::ifstream file;
	file.open("Resources/Data/csv/enemyPop.csv");
	assert(file.is_open());

	// �t�@�C���̓��e�𕶎��X�g���[���ɃR�s�[
	enemyData_ << file.rdbuf();

	// �t�@�C�������
	file.close();
}

std::vector<Vector3> GameScene::TrajectoryKind(uint8_t trajectoryKind)
{
	if (trajectoryKind == uint8_t(TrajectoryKinds::LEFT_2_RIGHT))
	{
		// �X�v���C������_
		float z = 40.0f;
		Vector3 start = { -10,  0, z };
		Vector3 p1 =	{  -5,  5, z };
		Vector3 p2 =    {	0,  0, z };
		Vector3 p3 =	{	5, -5, z };
		Vector3 end =	{  10,  0, z };

		std::vector<Vector3> movePoints = { start, p1, p2, p3, end };

		return movePoints;
	}
	else if (trajectoryKind == uint8_t(TrajectoryKinds::RIGHT_2_LEFT))
	{
		// �X�v���C������_
		float z = 40.0f;
		Vector3 start =	{  10,  0, z };
		Vector3 p1 =	{   5, -5, z };
		Vector3 p2 =	{   0,  0, z };
		Vector3 p3 =	{  -5,  5, z };
		Vector3 end =	{ -10,  0, z };

		std::vector<Vector3> movePoints = { start, p1, p2, p3, end };

		return movePoints;
	}
	else
	{
		// �X�v���C������_
		float z = 40.0f;
		Vector3 start = { 0, 0, z };
		Vector3 p1 =	{ 0, 0, z };
		Vector3 p2 =	{ 0, 0, z };
		Vector3 p3 =	{ 0, 0, z };
		Vector3 end =	{ 0, 0, z };

		std::vector<Vector3> movePoints = { start, p1, p2, p3, end };

		return movePoints;
	}
}

void GameScene::UpdateEnemyData()
{
	// �ҋ@����
	if (isStandBy_)
	{
		if (waitTimer_.GetActive() == false)
		{
			// �ҋ@�I��
			isStandBy_ = false;
		}
		return;
	}

	// 1�s���̕����������ϐ�
	std::string line;

	// �R�}���h���s���[�v
	while (getline(enemyData_, line))
	{
		// 1�s���̕��������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		// �J���}��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		// �R�����g�A�E�g
		if (word.find("//") == 0)
		{
			// �s���΂�
			continue;
		}

		// POP
		if (word.find("POP") == 0)
		{
			// �G�̎��
			getline(line_stream, word, ',');
			uint8_t kind = static_cast<uint8_t>(std::atoi(word.c_str()));

			// �G�̋O��
			getline(line_stream, word, ',');
			uint8_t trajectory = static_cast<uint8_t>(std::atoi(word.c_str()));

			EnemySpawn(kind, trajectory);
		}
		// WAIT�R�}���h
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// �҂�����
			float waitTimeData = float(std::atoi(word.c_str()));

			// �ҋ@�J�n
			isStandBy_ = true;
			waitTimer_.Start(waitTimeData);

			// �R�}���h���[�v�𔲂���
			break;
		}
	}
}