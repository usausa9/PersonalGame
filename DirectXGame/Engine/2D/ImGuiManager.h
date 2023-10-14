#pragma once

/**
 * @file	ImGuiManager.h
 * @brief	ImGuiのマネージャー
**/

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

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	// ImGui受付開始
	void Begin();

	// ImGui受付終了
	void End();

	// 描画
	void Draw();

private: // メンバ変数
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};