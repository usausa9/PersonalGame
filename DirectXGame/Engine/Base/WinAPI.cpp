#include "WinAPI.h"
#include <imgui_impl_win32.h>

#pragma comment(lib, "winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

WinAPI* WinAPI::GetInstance()
{
	if (sCurrentWindow_ == nullptr)
	{
		sCurrentWindow_ = new WinAPI;
	}
	return sCurrentWindow_;
}

LRESULT WinAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGui用ウィドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinAPI::Init()
{
	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	// ウィンドウクラスの設定
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;	// ウィンドウプロシージャを設定
	w_.lpszClassName = L"DirectXGame";		// ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);// カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassExW(&w_);
}

void WinAPI::Finalize()
{
	delete sCurrentWindow_;
	sCurrentWindow_ = nullptr;
}

bool WinAPI::ProcessMessage()
{
	// メッセージがある？
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_); // キー入力メッセージの処理
		DispatchMessage(&msg_);  // プロシージャにメッセージを送る  (こいつを呼ぶとウィンドウプロシージャが呼ばれるよ♡)
	}

	// ×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg_.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

void WinAPI::DebugText(LPCSTR text)
{
	// コンソールへの文字出力
	OutputDebugStringA(text);
}

void WinAPI::CreateWindowObj(LPCWSTR title, uint16_t windowWidth, uint16_t windowHeight)
{
	// ウィンドウサイズ
	width_ = windowWidth;
	height_ = windowHeight;

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	wrc_ = { 0, 0, width_, height_ };

	// 自動でサイズを補正する
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(
		w_.lpszClassName,		// クラス名
		title,					// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// 標準的なウィンドウスタイル
		CW_USEDEFAULT,			// 表示X座標 (OSに任せる)
		CW_USEDEFAULT,			// 表示Y座標 (OSに任せる)
		wrc_.right - wrc_.left,	// ウィンドウ横幅
		wrc_.bottom - wrc_.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		w_.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);				// オプション
}

void WinAPI::Show()
{
	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);
}

WinAPI::WinAPI() {}

WinAPI::~WinAPI() {}

WinAPI* WinAPI::sCurrentWindow_;