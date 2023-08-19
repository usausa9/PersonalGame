#include "ImGuiManager.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize()
{
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();

	// ImGuiのスタイル設定
	ImGui::StyleColorsDark();

	// デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// デスクリプタヒープ生成
	HRESULT result = S_FALSE;
	result = DirectXBase::GetInstance()->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// Win32用の初期関数
	ImGui_ImplWin32_Init(WinAPI::GetInstance()->GetHwnd());
	// DirectX12用の初期関数
	ImGui_ImplDX12_Init(
		DirectXBase::GetInstance()->GetDevice(),
		static_cast<int>(DirectXBase::GetInstance()->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	// 標準フォントを追加
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// ヒープ解法
	srvHeap_.Reset();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = {srvHeap_.Get()};
	DirectXBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXBase::GetInstance()->GetCommandList());
}