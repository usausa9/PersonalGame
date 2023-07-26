#pragma once

#include "WinAPI.h"
#include "DirectXBase.h"

class ImGuiManager
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	// メンバ関数

	// シングルトンインスタンスを取得
	static ImGuiManager* GetInstance();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private: // メンバ変数

	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

