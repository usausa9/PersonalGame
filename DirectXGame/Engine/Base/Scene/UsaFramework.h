#pragma once
#include "Common.h"

#include "IScene.h"
#include "SceneManager.h"

#include "Input.h"
#include "WinAPI.h"

#include "SpriteManager.h"
#include "ImGuiManager.h"

#include "AbstractSceneFactory.h"

class UsaFramework
{
public:
	SceneManager* sceneManager_ = nullptr;

public:
	// ������
	virtual void Initialize();

	// �I��
	virtual void Finalize();

	// �X�V����
	virtual void Update();

	// �X�V�㏈��
	void PostUpdate();

	// �`��O����
	void PreDraw();

	// �p�[�e�B�N���`��O����
	void PreDrawParticle();

	// �`��
	// �������z�֐��ɂ��邱�ƂŃI�[�o�[���C�h��K�{��
	virtual void Draw(); 

	// �`��㏈��
	void PostDraw();

	// �I���`�F�b�N
	virtual bool IsEndRequest() { return endRequest_; }

	// ���z�f�X�g���N�^
	virtual ~UsaFramework() = default;

public:
	// ���s
	void Run();

protected: // �����o�ϐ�
	// �V�[���H��
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// �X�v���C�g�}�l�[�W��
	unique_ptr<SpriteManager> spriteManager_ = nullptr;

	// �I���t���O
	bool endRequest_ = false;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	// WindowsAPI������
	WinAPI& window_ = *WinAPI::GetInstance();
};

