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
	// ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();

	// ImGui�̃X�^�C���ݒ�
	ImGui::StyleColorsDark();

	// �f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// �f�X�N���v�^�q�[�v����
	HRESULT result = S_FALSE;
	result = DirectXBase::GetInstance()->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// Win32�p�̏����֐�
	ImGui_ImplWin32_Init(WinAPI::GetInstance()->GetHwnd());
	// DirectX12�p�̏����֐�
	ImGui_ImplDX12_Init(
		DirectXBase::GetInstance()->GetDevice(),
		static_cast<int>(DirectXBase::GetInstance()->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
		srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();
	// �W���t�H���g��ǉ�
	io.Fonts->AddFontDefault();
}
