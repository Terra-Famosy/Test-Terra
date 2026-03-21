#include <iostream>
#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>

struct GameData
{
	GameMap gameMap;
	Camera2D camrea;

}gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	gameData.gameMap.create(30, 10);

	gameData.gameMap.getBlockUnsafe(0, 0).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(1, 1).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(2, 2).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(4, 4).type = Block::dirt;

	gameData.camrea.target = { 0, 0 }; // world-space center of view
	gameData.camrea.rotation = 0.0f;
	gameData.camrea.zoom = 100.0f;

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	gameData.camrea.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

#pragma region camera movement
	if (IsKeyDown(KEY_A)) gameData.camrea.target.x -= 7.f * deltaTime;
	if (IsKeyDown(KEY_D)) gameData.camrea.target.x += 7.f * deltaTime;
	if (IsKeyDown(KEY_W)) gameData.camrea.target.y -= 7.f * deltaTime;
	if (IsKeyDown(KEY_S)) gameData.camrea.target.y += 7.f * deltaTime;
#pragma endregion

	BeginMode2D(gameData.camrea);

	for (int y = 0; y < gameData.gameMap.h; y++)
		for (int x = 0; x < gameData.gameMap.w; x++)
		{
			auto& b = gameData.gameMap.getBlockUnsafe(x, y);

			if (b.type != Block::air)
			{
				float size = 1;
				float posX = x * size;
				float posY = y * size;


				DrawTexturePro(
					assetManager.dirt, 
					Rectangle{0.f ,0.f, (float)assetManager.dirt.width, (float)assetManager.dirt.height }, // source
					{posX, posY, size, size}, // dest
					{0, 0}, // origin (top-left corner)
					0.0f, // rotation
					WHITE // tint
				);
			}
		}

	EndMode2D();


	return !IsKeyDown(KEY_ESCAPE);
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
	//std::ofstream f(RESOURCES_PATH "f.txt");
	//f << "\nCLOSED\n";
	//f.close();
}