#pragma once
#include "Common.h"

#include "Input.h"
#include "WinAPI.h"
#include "DirectXBase.h"

#include "ParticleManager.h"
#include "SpriteManager.h"

#include "UsaFramework.h"
#include "FbxLoader.h"

#include "GameScene.h"

#include "ImGuiManager.h"

#include <Windows.h>
#include <cmath>

#include <DirectXTex.h>

class MyGame /*: public UsaFramework*/
{
public:
	GameScene gameScene;

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	// �����o�֐�
	// ������
	void Initialize();

	// �I��
	void Finalize();

	// ���t���[���X�V
	void Update();

	// �`��O����
	void PreDraw();

	// �p�[�e�B�N���`��O����
	void PreDrawParticle();

	// �`��
	void Draw();

	// �`��㏈��
	void PostDraw();

	// �I���t���O�`�F�b�N
	bool IsEndRequest() { return endRequest_; }

private: // �����o�ϐ�

	unique_ptr<SpriteManager> spriteManager = nullptr;

	// �I���t���O
	bool endRequest_ = false;
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	// WindowsAPI������
	WinAPI& window = *WinAPI::GetInstance();

	// ImGui
	unique_ptr<ImGuiManager> imGui = nullptr;
};

