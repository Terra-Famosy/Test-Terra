#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>


int main(void)
{

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "window name");


#pragma region imgui
	rlImGuiSetup(true);


	ImGuiIO& io = ImGui::GetIO();
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;       // Enable gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 2;

#pragma endregion


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

	#pragma region imgui
		rlImGuiBegin();

		//docking stuff
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
	#pragma endregion

		Color c;
		c.r = 255;
		c.g = 0;
		c.b = 200;
		c.a = 255;

		DrawText("Congrats! You created your first window!", 190, 200, 20, c);


		ImGui::Begin("test");

		ImGui::Text("hello");

		if (ImGui::Button("button"))
		{
			std::cout << "Text\n";
		}
		ImGui::SameLine();
		if (ImGui::Button("button2"))
		{
			std::cout << "Different button\n";
		}

		ImGui::End();

		ImGui::Begin("second window");

		ImGui::Text("hello");
		ImGui::Separator();
		ImGui::NewLine();
		static float a = 0;
		ImGui::SliderFloat("slider", &a, 0, 1);

		ImGui::End();

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