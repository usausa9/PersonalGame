#include "TitleScene.h"
#include "CollisionManager.h"

#include "FBXObject3D.h"

#include "ImGuiManager.h"

using namespace Input;

void TitleScene::Initialize()
{
	// �����蔻��
	collisionManager = CollisionManager::GetInstance();

	// �p�[�e�B�N���p�̃p�C�v���C���EInit
	ParticleManager::CreatePipeline();

	// �J����������
	camera = new Camera;
	camera->Initialize();

	camera->target = { 0,0,0 };
	camera->position = { 0,0,-20.0f };

	// �v���C���[������
	player = make_unique<Player>();
	player.get()->Initialize(camera);

	// �G�l�~�[������
	enemy = make_unique<Enemy>();
	enemy.get()->Initialize();

	// �V��������
	skydome = make_unique<Skydome>();
	skydome.get()->Initialize();
}

void TitleScene::Finalize()
{
	delete camera;
}

void TitleScene::Update()
{
	// DIrectX���t���[������(�X�V����) ��������

	// �v���C���[�̍X�V
	player->Update();

	// �G�l�~�[�̍X�V
	enemy->Update();

	// �V���̍s��X�V
	skydome->Update();

	// �J�����̍X�V
	camera->Update();

	// �S�Ă̏Փ˂��`�F�b�N (�X�V�̍Ō�)
	collisionManager->CheckAllCollisions();
}

void TitleScene::Draw3D()
{
	// �J�����Z�b�g
	camera->Set();

	// �V���`��
	skydome->Draw();

	// �v���C���[�`��
	player->Draw();

	// �G�`��
	/*enemy->Draw();*/
}

void TitleScene::DrawParticle()
{
	// �J�����Z�b�g
	camera->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void TitleScene::Draw2D()
{
	
}