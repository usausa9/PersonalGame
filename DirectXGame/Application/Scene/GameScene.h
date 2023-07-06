#pragma once
#include "Input.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "DirectXBase.h"

#include "Player.h"

#include "FBXObject3D.h"
#include "Object3D.h"
#include "Camera.h"

#include "OBJModel.h"
#include "FBXModel.h"

#include "FbxLoader.h"

class GameScene
{
private:
	// �e�N�X�`��

	// �X�v���C�g

	// �p�[�e�B�N��

	// OBJ���f��

	// Object3D

	// FBX

	// �J����
	Camera* camera = new Camera;

	unique_ptr<Player> player = nullptr;

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
};