#pragma once
#include <windows.h>
#include <string>

class WinAPI
{
public:
	// �E�B���h�E�T�C�Y
	uint16_t width_ = 1280;
	uint16_t height_ = 720;
	
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w_{};
	// �E�B���h�E�n���h���̐���
	HWND hwnd_{};
	// �E�B���h�E�T�C�Y�p�̒����`�̐���
	RECT wrc_{};
	// ���b�Z�[�W���\���̂̐���
	MSG msg_{};

public:
	// �V���O���g���C���X�^���X
	static WinAPI* GetInstance();

	// �E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// Window�N���X�̐ݒ�
	void Init();

	// �I������
	static void Finalize();

	// �X�V����
	bool ProcessMessage();

	// �R���\�[���ւ̕����o��
	void DebugText(LPCSTR text);

	// �E�B���h�E�I�u�W�F�N�g�̐���
	void CreateWindowObj(LPCWSTR title, uint16_t windowWidth, uint16_t windowHeight);

	// �E�B���h�E�\��
	void Show();

	// �E�B���h�E�I�u�W�F���g�擾
	HWND GetHwnd() const { return hwnd_; }

public:
	static WinAPI* sCurrentWindow_;

private:
	// �V���O���g���Ȃ̂Ńv���C�x�[�g�ɋL�q
	WinAPI();
	~WinAPI();
	WinAPI(const WinAPI&);
	WinAPI& operator=(const WinAPI&);
};