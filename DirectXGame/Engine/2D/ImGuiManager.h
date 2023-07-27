#pragma once

#include "WinAPI.h"
#include "DirectXBase.h"

class ImGuiManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	// �����o�֐�

	// �V���O���g���C���X�^���X���擾
	static ImGuiManager* GetInstance();
	
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	// ImGui��t�J�n
	void Begin();

	// ImGui��t�I��
	void End();

	// �`��
	void Draw();

private: // �����o�ϐ�

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

