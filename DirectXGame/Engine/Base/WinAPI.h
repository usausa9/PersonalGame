#pragma once
#include <windows.h>
#include <string>

class WinAPI
{
public:
	// ウィンドウサイズ
	uint16_t width_ = 1280;
	uint16_t height_ = 720;
	
	// ウィンドウクラスの設定
	WNDCLASSEX w_{};
	// ウィンドウハンドルの生成
	HWND hwnd_{};
	// ウィンドウサイズ用の長方形の生成
	RECT wrc_{};
	// メッセージ情報構造体の生成
	MSG msg_{};

public:
	// シングルトンインスタンス
	static WinAPI* GetInstance();

	// ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// Windowクラスの設定
	void Init();

	// 終了処理
	static void Finalize();

	// 更新処理
	bool ProcessMessage();

	// コンソールへの文字出力
	void DebugText(LPCSTR text);

	// ウィンドウオブジェクトの生成
	void CreateWindowObj(LPCWSTR title, uint16_t windowWidth, uint16_t windowHeight);

	// ウィンドウ表示
	void Show();

	// ウィンドウオブジェくト取得
	HWND GetHwnd() const { return hwnd_; }

public:
	static WinAPI* sCurrentWindow_;

private:
	// シングルトンなのでプライベートに記述
	WinAPI();
	~WinAPI();
	WinAPI(const WinAPI&);
	WinAPI& operator=(const WinAPI&);
};