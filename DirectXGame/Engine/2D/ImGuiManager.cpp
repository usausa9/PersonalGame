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

void ImGuiManager::Finalize()
{
	// ��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �q�[�v��@
	srvHeap_.Reset();
}

void ImGuiManager::Begin()
{
	// ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// �`��O����
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = {srvHeap_.Get()};
	DirectXBase::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	
	// �`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXBase::GetInstance()->GetCommandList());
}