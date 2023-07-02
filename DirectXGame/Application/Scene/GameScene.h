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
	OBJModel vicviper;
	OBJModel ico;

	// Object3D
	Object3D playerObj;
	Object3D playerBulletObj;

	bool bulletLive = false;
	// FBX

	// �J����
	Camera* camera = new Camera;

	unique_ptr<Player> player = nullptr;
	

	float velocity = 0.7f;

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