#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// ���V�[���𐶐�
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::Update()
{
	// �؂�ւ��@�\

	// ���V�[���̗\�񂪂���Ȃ�
	if (nextScene_)
	{
		// ���V�[���̏I��
		if (currentScene_)
		{
			// ���݃V�[���̏I������
			currentScene_->Finalize();
			delete currentScene_;
		}

		// �V�[���؂�ւ�
		currentScene_ = nextScene_;
		nextScene_ = nullptr;

		// �V�[���}�l�[�W�����Z�b�g
		currentScene_->SetSceneManager(this);

		// ���V�[���̏�����
		currentScene_->Initialize();
	}

	// ���s���V�[�����X�V����
	currentScene_->Update();
}

void SceneManager::Draw3D()
{
	// 3D�`��
	currentScene_->Draw3D();
}

void SceneManager::DrawParticle()
{
	// �p�[�e�B�N���`��
	currentScene_->DrawParticle();
}

void SceneManager::Draw2D()
{
	// 2D�`��
	currentScene_->Draw2D();
}

SceneManager::~SceneManager()
{
	// �Ō�̃V�[���̏I���Ɖ��
	currentScene_->Finalize();
	delete currentScene_;
}