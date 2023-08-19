#pragma once
#include "AbstractSceneFactory.h"

// �V�[���Ǘ�
class SceneManager
{
public:
	// �V���O���g���C���X�^���X���擾
	static SceneManager* GetInstance();

	// ���V�[���̗\��
	void ChangeScene(const std::string& sceneName);

	// �V�[���t�@�N�g���[�̃Z�b�^�[
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
private:
	// ���݂̃V�[��
	IScene* currentScene_ = nullptr;
	IScene* nextScene_ = nullptr;

	// �V�[���H�� (�؂����)
	AbstractSceneFactory* sceneFactory_ = nullptr;

public: // �����o�֐�
	// �X�V����
	void Update();

	// 3D�`��
	void Draw3D();

	// �p�[�e�B�N���`��
	void DrawParticle();

	// 2D�`��
	void Draw2D();
	
	// �f�X�g���N�^
	~SceneManager();
};