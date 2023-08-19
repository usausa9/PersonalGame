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

// DirectX���
class DirectXBase
{
private:
	HRESULT result = S_FALSE;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �L�^����(FPS�Œ�p)
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
	ComPtr<ID3D12Device> device_ = nullptr; // �f�o�C�X
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr; // �R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr; // �R�}���h���X�g
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr; // �R�}���h�L���[
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr; // �X���b�v�`�F�[��
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr; // �����_�[�^�[�Q�b�g�r���[

	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr; // ����]

	inline ID3D12Device* GetDevice() const { return device_.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList()const { return commandList_.Get(); }

	size_t GetBackBufferCount() const { return backBuffers_.size(); }
public:
	// �V���O���g���C���X�^���X���擾
	static DirectXBase* GetInstance();

	// ������
	void Init();

	// �I������
	static void Finalize();

	// �`��O����
	void PreDraw();

	// �`��㏈��
	void PostDraw();

private:
	// �V���O���g���Ȃ̂Ńv���C�x�[�g�ɋL�q
	DirectXBase();
	~DirectXBase();
	DirectXBase(const DirectXBase&);
	DirectXBase& operator=(const DirectXBase&);

	// FPS�Œ菉����
	void InitializeFixFPS();

	// FPS�Œ�X�V
	void UpdateFixFPS();

public:
	static DirectXBase* sCurrentDirectX_;
};