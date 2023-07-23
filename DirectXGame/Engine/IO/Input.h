#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// ComPtr
#include <wrl.h>

#include "Common.h"

enum class Button
{
	UP = 0x0001,
	DOWN = 0x0002,
	LEFT = 0x0004,
	RIGHT = 0x0008,
	START = 0x0010,
	BACK = 0x0020,
	LSTICKBUTTON = 0x0040,
	RSTICKBUTTON = 0x0080,
	L = 0x0100,
	R = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
};

namespace Input
{
	class Key
	{
	public:
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		// �������ƃA�b�v�f�[�g�̊֐� �p��
		static void Init(HINSTANCE hInstance, HWND hwnd);
		static void Update();

		// �L�[�{�[�h���͏����p (�Ԃ�l0,1)
		static bool Down(UINT8 keyNum);		// �������ςȂ�
		static bool Trigger(UINT8 keyNum);	// �������u��
		static bool Released(UINT8 keyNum);	// �������u��

	private: // �����o�ϐ�
		static ComPtr<IDirectInputDevice8> keyboard;
	};

	class Pad
	{
	public:
		static void Init();
		static void Update();

		static bool Down(Button button);		// �������ςȂ�
		static bool Trigger(Button button);		// �������u��
		static bool Released(Button button);	// �������u��

		static Float2 GetLStick();
		static Float2 GetRStick();

	private:
		static XINPUT_STATE padState;
		static XINPUT_STATE lastPadState;
	};
}