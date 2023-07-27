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
	std::vector<ID3D12Resource*> backBuffers;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	D3D12_RESOURCE_BARRIER barrierDesc{};
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	ComPtr<ID3D12Resource> depthBuff = nullptr;

public:
	ComPtr<ID3D12Device> device = nullptr; // �f�o�C�X
	ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr; // �R�}���h�A���P�[�^
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr; // �R�}���h���X�g
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr; // �R�}���h�L���[
	ComPtr<IDXGISwapChain4> swapChain = nullptr; // �X���b�v�`�F�[��
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr; // �����_�[�^�[�Q�b�g�r���[

	ComPtr<IDXGIFactory7> dxgiFactory = nullptr; // ����]

	inline ID3D12Device* GetDevice() const { return device.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); }

	size_t GetBackBufferCount() const { return backBuffers.size(); }
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
	// FPS�Œ菉����
	void InitializeFixFPS();

	// FPS�Œ�X�V
	void UpdateFixFPS();

public:
	// �Q�b�^�[
	/*static DirectXBase* Get();*/
	static DirectXBase* currentDirectX;
};