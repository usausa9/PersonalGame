#pragma once

// �O���錾
class SceneManager;

class IScene
{
public:
	// ������
	virtual void Initialize() = 0;

	// �I��
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update() = 0;

	// 3D�`��
	virtual void Draw3D() = 0;

	// �p�[�e�B�N���`��
	virtual void DrawParticle() = 0;

	// 2D�`��
	virtual void Draw2D() = 0;

public: // �����o�֐�
	virtual ~IScene() = default;

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:
	// �V�[���}�l�[�W�� (�؂����)
	SceneManager* sceneManager_ = nullptr;
};