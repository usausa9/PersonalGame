#include "GameScene.h"

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

	// �v���C���[������
	player = make_unique<Player>();
	player.get()->Initialize();
	
	// ���[���J����������
	railCamera = new RailCamera();
	railCamera->Initialize({0, 0, -20.0f}, {0, 0, 0});

	// �V��������
	skydome = make_unique<Skydome>();
	skydome.get()->Initialize();
}

void GameScene::Finalize()
{
	//delete camera;
	delete railCamera;
}

void GameScene::Update()
{
	// DIrectX���t���[������(�X�V����) ��������
	railCamera->Update();

	// �v���C���[�̍X�V
	player->SetParent(railCamera->GetObject3d());
	player->Update();

	// 
	skydome->Update();

	//// �A���[�L�[�ŃJ�����ړ�
	//if (Key::Down(DIK_LEFT))
	//{
	//	camera->position.x -= 0.5f;
	//}
	//else if (Key::Down(DIK_RIGHT))
	//{
	//	camera->position.x += 0.5f;
	//}

	//if (Key::Down(DIK_DOWN))
	//{
	//	camera->position.y -= 0.5f;
	//}
	//else if (Key::Down(DIK_UP))
	//{
	//	camera->position.y += 0.5f;
	//}
	
	// �J���������[���J�����̂��̂�
	camera = railCamera->GetCamera();
	camera->Update();
}

void GameScene::Draw3D()
{
	// �J�����Z�b�g
	camera->Set();

	// �V���`��
	skydome->Draw();

	// �v���C���[�`��
	player.get()->Draw();
}

void GameScene::DrawParticle()
{
	// �J�����Z�b�g
	camera->Set();

	// �p�[�e�B�N���I�u�W�F�`��
}

void GameScene::Draw2D()
{
}