#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <cassert>
#include <vector>
#include <chrono>
#include <thread>

#pragma comment (lib,"d3d12.lib")
#pragma comment (lib,"dxgi.lib")

// DirectX基盤
class DirectXBase
{
private:
	HRESULT result = S_FALSE;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

public:
	std::vector<ID3D12Resource*> backBuffers_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	D3D12_RESOURCE_BARRIER barrierDesc_{};
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceVal_ = 0;

	ComPtr<ID3D12Resource> depthBuff_ = nullptr;

public:
	ComPtr<ID3D12Device> device_ = nullptr; // デバイス
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr; // コマンドアロケータ
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr; // コマンドリスト
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr; // コマンドキュー
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr; // スワップチェーン
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr; // レンダーターゲットビュー

	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr; // 統合]

	inline ID3D12Device* GetDevice() const { return device_.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }

	size_t GetBackBufferCount() const { return backBuffers_.size(); }
public:
	// シングルトンインスタンスを取得
	static DirectXBase* GetInstance();

	// 初期化
	void Init();

	// 終了処理
	static void Finalize();

	// 描画前処理
	void PreDraw();

	// 描画後処理
	void PostDraw();

private:
	// シングルトンなのでプライベートに記述
	DirectXBase();
	~DirectXBase();
	DirectXBase(const DirectXBase&);
	DirectXBase& operator=(const DirectXBase&);

	// FPS固定初期化
	void InitializeFixFPS();

	// FPS固定更新
	void UpdateFixFPS();

public:
	static DirectXBase* sCurrentDirectX_;
};