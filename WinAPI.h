#pragma once
#include <windows.h>
#include <string>

class WinAPI
{
public:
	// �E�B���h�E�T�C�Y
	int width = 1280;
	int height = 720;
	
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�n���h���̐���
	HWND hwnd;
	// �E�B���h�E�T�C�Y�p�̒����`�̐���
	RECT wrc;
	// ���b�Z�[�W���\���̂̐���
	MSG msg = {};

public:
	// �E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// Window�N���X�̐ݒ�
	void Init();

	// �X�V����
	bool ProcessMessage();

	// �R���\�[���ւ̕����o��
	void DebugText(LPCSTR text);

	// �E�B���h�E�I�u�W�F�N�g�̐���
	void CreateWindowObj(LPCWSTR title, int windowWidth, int windowHeight);

	// �E�B���h�E�\��
	void Show();

public:
	// �Q�b�^�[
	static WinAPI* Get();

private:
	static WinAPI currentWindow;
};