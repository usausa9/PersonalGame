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
	vicviper = OBJModel::LoadFromOBJ("vicviper");
	ico = OBJModel::LoadFromOBJ("ICO");

	// Object3D Init
	playerObj.position = { 0, 0, 0 };
	playerObj.scale = { 1, 1, 1 };
	playerObj.rotation = { 20 * (UsaMath::u_PI / 180), 0, 0};
	playerObj.InitializeObject3D();

	playerBulletObj.position = { -100.0f, 0, 0 };
	playerBulletObj.InitializeObject3D();
	playerBulletObj.objModel = &ico;

	// obj��Object3D�̕R�t��
	player = make_unique<Player>();
	player.get()->Initialize();
	playerObj.objModel = &vicviper;

	// FBX���f���֘A
	// ���f�������w�肵��FBX�t�@�C���ǂݍ���

	// �J����������
	camera->Initialize();

	camera->target = { 0,0,0 };
	camera->position = { 0,0,20.0f };
}

void GameScene::Finalize()
{
	delete camera;
}

void GameScene::Update()
{
	// DIrectX���t���[������(�X�V����) ��������

	player.get()->Update();

	playerObj.UpdateObject3D();
	playerBulletObj.UpdateObject3D();

	if (Key::Down(DIK_D) && Key::Down(DIK_A))
	{

	}
	else if (Key::Down(DIK_D) && playerObj.position.x >= -25.0f)
	{
		playerObj.position += { -velocity, 0, 0 };
	}
	else if (Key::Down(DIK_A) && playerObj.position.x <= 25.0f)
	{
		playerObj.position += { velocity, 0, 0 };
	}


	if (Key::Down(DIK_W) && Key::Down(DIK_S))
	{

	}
	else if (Key::Down(DIK_W) && playerObj.position.y <= 13.0f)
	{
		playerObj.position += { 0, velocity * 9 / 14, 0 };
	}
	else if (Key::Down(DIK_S) && playerObj.position.y >= -13.0f)
	{
		playerObj.position += { 0, -velocity * 9 / 14, 0 };
	}

	if (Key::Trigger(DIK_SPACE))
	{
		bulletLive = true;
		playerBulletObj.position = playerObj.position + Vector3{0, 0.6f, -5.9f};
	}

	if (bulletLive == true)
	{
		playerBulletObj.position -= { 0, 0, 1.3f };
	}

	if (playerBulletObj.position.z < -50.0f)
	{
		bulletLive = false;
	}

	if (Key::Down(DIK_LEFT))
	{
		camera->position.x -= 0.5f;
	}
	else if (Key::Down(DIK_RIGHT))
	{
		camera->position.x += 0.5f;
	}

	if (Key::Down(DIK_DOWN))
	{
		camera->position.y -= 0.5f;
	}
	else if (Key::Down(DIK_UP))
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
	//playerObj.DrawObject3D();
	player.get()->Draw();

	if (bulletLive == true)
	{
		playerBulletObj.DrawObject3D();
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
}