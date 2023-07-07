#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//// DirectInput�̏�����
//static ComPtr<IDirectInput8> directInput = nullptr;
//static ComPtr<IDirectInputDevice8> keyboard = nullptr;

using namespace Input;

Microsoft::WRL::ComPtr<IDirectInputDevice8> Key::keyboard = nullptr;

XINPUT_STATE Pad::padState;
XINPUT_STATE Pad::lastPadState;

// �S�L�[�̓��͏�Ԃ��擾����
static BYTE _keys[256] = {};

// �S�L�[��1F�O�̓��͏�Ԃ��擾����
static BYTE _prev[256] = {};

void Key::Init(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInput�̏�����
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Key::Update()
{
	// �O��X�V
	for (int i = 0; i < 256; ++i)
	{
		_prev[i] = _keys[i];
	}

	// �L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(_keys), _keys);
}

// �������ςȂ�
bool Key::Down(UINT8 keyNum)
{
	return _keys[keyNum];
}

// �������u��
bool Key::Trigger(UINT8 keyNum)
{
	return _keys[keyNum] && !_prev[keyNum];
}

// �������u��
bool Key::Released(UINT8 keyNum)
{
	return !_keys[keyNum] && !_prev[keyNum];
}

void Pad::Init()
{

}

void Pad::Update()
{
	lastPadState = padState;

	XInputGetState(0, &padState);
}

bool Pad::Down(Button button)
{
	return padState.Gamepad.wButtons & (UINT)button;
}

bool Pad::Trigger(Button button)
{
	return padState.Gamepad.wButtons & (UINT)button && !(lastPadState.Gamepad.wButtons & (UINT)button);
}

bool Pad::Released(Button button)
{
	return lastPadState.Gamepad.wButtons & (UINT)button && !(padState.Gamepad.wButtons & (UINT)button);
}

Float2 Pad::GetLStick()
{
	float x = padState.Gamepad.sThumbLX;
	float y = padState.Gamepad.sThumbLY;

	if (x * x < 5000 * 5000)
	{
		x = 0;
	}
	if (y * y < 5000 * 5000)
	{
		y = 0;
	}

	x = x / 32767;
	y = y / 32767;

	return { x, y };
}

Float2 Pad::GetRStick()
{
	float x = padState.Gamepad.sThumbRX;
	float y = padState.Gamepad.sThumbRY;

	x = x / 32767;
	y = y / 32767;

	return { x, y };
}