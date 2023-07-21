#include "GameScene.h"
#include "CollisionManager.h"

#include "FBXObject3D.h"

using namespace Input;

void GameScene::Initialize()
{
	// �f�o�C�X���Z�b�g
	FBXObject3D::SetDevice(DirectXBase::Get()->device.Get());
	// �J�������Z�b�g
	FBXObject3D::SetCamera(camera);
	// �R�}���h���X�g������
	FBXObject3D::SetCommandList(DirectXBase::Get()->commandList.Get());
	// �O���t�B�b�N�X�p�C�v���C������
	FBXObject3D::CreateGraphicsPipeline();

	// �����蔻��
	collisionManager = CollisionManager::GetInstance();

	
	
	// �e�N�X�`���ǂݍ���

	// �X�v���C�g�E�e�N�X�`���R�Â�

	// �p�[�e�B�N���p�̃p�C�v���C���EInit
	ParticleManager::CreatePipeline();

	// OBJ�ǂݍ���

	// Object3D Init

	// obj��Object3D�̕R�t��

	// �J����������
	camera = new Camera;
	camera->Initialize();

	// ���[���J����������
	railCamera = new RailCamera();
	railCamera->Initialize({ 0, 0, -20.0f }, { 0, 0, 0 });

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
	enemy->Update();

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
	enemy->Draw();
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