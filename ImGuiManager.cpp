#include "ImGuiManager.h"

#include <imgui_impl_win32.h>

void ImGuiManager::Initialize()
{
	// ImGuiのコンテキストを生成
	ImGui::CreateContext();

	// ImGuiのスタイル設定
	ImGui::StyleColorsDark();
}
