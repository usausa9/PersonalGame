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

	// FBX���f���֘A

	// �J����������
	camera->Initialize();

	//camera->target = { 0,20,0 };
	//camera->position = { 0,0,-100 };
}

void GameScene::Finalize()
{

}

void GameScene::Update()
{

	if (Key::Down(DIK_A) || Key::Down(DIK_LEFT))
	{
		camera->position.x -= 0.5f;
	}
	else if (Key::Down(DIK_D) || Key::Down(DIK_RIGHT))
	{
		camera->position.x += 0.5f;
	}

	if (Key::Down(DIK_S) || Key::Down(DIK_DOWN))
	{
		camera->position.y -= 0.5f;
	}
	else if (Key::Down(DIK_W) || Key::Down(DIK_UP))
	{
		camera->position.y += 0.5f;
	}
	
	camera->Update();
}

void GameScene::Draw3D()
{
	// �J�����Z�b�g
	camera->Set();

	// 3D�I�u�W�F�`��

}

void GameScene::DrawParticle()
{
	// �J�����Z�b�g
	camera->Set();

	// �p�[�e�B�N���`��
}

void GameScene::Draw2D()
{
	// �O�i�X�v���C�g�`��
}