#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//// DirectInputの初期化
//static ComPtr<IDirectInput8> directInput = nullptr;
//static ComPtr<IDirectInputDevice8> keyboard = nullptr;

using namespace Input;

Microsoft::WRL::ComPtr<IDirectInputDevice8> Key::sKeyboard_ = nullptr;

XINPUT_STATE Pad::sPadState_;
XINPUT_STATE Pad::sLastPadState_;

// 全キーの入力状態を取得する
static BYTE keys[256] = {};

// 全キーの1F前の入力状態を取得する
static BYTE prevKeys[256] = {};

void Key::Init(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &sKeyboard_, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = sKeyboard_->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = sKeyboard_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Key::Update()
{
	// 前後更新
	for (int i = 0; i < 256; ++i)
	{
		prevKeys[i] = keys[i];
	}

	// キーボード情報の取得開始
	sKeyboard_->Acquire();
	sKeyboard_->GetDeviceState(sizeof(keys), keys);
}

// 押しっぱなし
bool Key::Down(UINT8 keyNum)
{
	return keys[keyNum];
}

// 押した瞬間
bool Key::Trigger(UINT8 keyNum)
{
	return keys[keyNum] && !prevKeys[keyNum];
}

// 離した瞬間
bool Key::Released(UINT8 keyNum)
{
	return !keys[keyNum] && !prevKeys[keyNum];
}

void Pad::Init()
{

}

void Pad::Update()
{
	sLastPadState_ = sPadState_;

	XInputGetState(0, &sPadState_);
}

bool Pad::Down(Button button)
{
	return sPadState_.Gamepad.wButtons & (UINT)button;
}

bool Pad::Trigger(Button button)
{
	return sPadState_.Gamepad.wButtons & (UINT)button && !(sLastPadState_.Gamepad.wButtons & (UINT)button);
}

bool Pad::Released(Button button)
{
	return sLastPadState_.Gamepad.wButtons & (UINT)button && !(sPadState_.Gamepad.wButtons & (UINT)button);
}

Vector2 Pad::GetLStick()
{
	float x = sPadState_.Gamepad.sThumbLX;
	float y = sPadState_.Gamepad.sThumbLY;

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

Vector2 Pad::GetRStick()
{
	float x = sPadState_.Gamepad.sThumbRX;
	float y = sPadState_.Gamepad.sThumbRY;

	x = x / 32767;
	y = y / 32767;

	return { x, y };
}