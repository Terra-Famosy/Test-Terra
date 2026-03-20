#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>
#include <gameMain.h>


int main(void)
{
#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE); // no log output to the console by raylib
#endif


	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "window name");
	SetExitKey(KEY_NULL);
	SetTargetFPS(240);


#pragma region imgui
	rlImGuiSetup(true);


	ImGuiIO& io = ImGui::GetIO();
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;       // Enable gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 2;

#pragma endregion

	if (!initGame())
	{
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

	#pragma region imgui
		rlImGuiBegin();

		//docking stuff
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
	#pragma endregion


		if (!updateGame()) 
		{
			closeGame();
			break;
		}


		//ImGui::ShowDemoWindow();

	#pragma region imgui
		rlImGuiEnd();

	#pragma endregion

		EndDrawing();
	}

#pragma region imgui
	rlImGuiShutdown();
#pragma endregion

	CloseWindow();

	return 0;
}