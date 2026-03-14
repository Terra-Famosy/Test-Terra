#include <iostream>
#include <raylib.h>
#include <imgui.h>


int main()
{

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Test-Terra");


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangle(75, 75, 100, 100, { 0, 255, 0, 127 });
		DrawRectangle(50, 50, 100, 100, { 255, 0, 0, 127 });

		Color c;
		c.r = 255;
		c.g = 0;
		c.b = 200;
		c.a = 255;

		DrawText("Congrats! You created your first window!", 190, 200, 20, c);



		EndDrawing();
	}


	CloseWindow();

	return 0;
}