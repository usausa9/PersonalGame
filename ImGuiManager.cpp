#include "ImGuiManager.h"

#include <imgui_impl_win32.h>

void ImGuiManager::Initialize()
{
	// ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();

	// ImGui�̃X�^�C���ݒ�
	ImGui::StyleColorsDark();
}
