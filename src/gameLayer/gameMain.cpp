#include <iostream>
#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>

struct GameData
{
	GameMap gameMap;
	Camera2D camrea;

}gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	gameData.gameMap.create(700, 200);

	for (int y = 0; y < gameData.gameMap.h; y++)
		for (int x = 0; x < gameData.gameMap.w; x++)
		{

			float s = (std::sin(x) + 1.f) / 2.f;
			float s2 = (std::sin(x * 0.5) + 1.f) / 2.f;

			if (gameData.gameMap.h - (gameData.gameMap.h * 0.3 * s) - gameData.gameMap.h * 0.5 - (gameData.gameMap.h * 0.2 * s2) < y)
			{
				gameData.gameMap.getBlockUnsafe(x, y).type = Block::dirt;
			}
			else
			{
				gameData.gameMap.getBlockUnsafe(x, y).type = Block::air;
			}
		}

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

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camrea);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b)
		{
			*b = {};
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b)
		{
			b->type = Block::gold;
		}
	}

#pragma region draw world

	BeginMode2D(gameData.camrea);

	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, gameData.camrea);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight()}, gameData.camrea);

	int startXView = (int)floor(topLeftView.x - 1);
	int endXView = (int)floor(bottomRightView.x + 1);
	int startYView = (int)floor(topLeftView.y - 1);
	int endYView = (int)floor(bottomRightView.y + 1);

	startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);

	for (int y = startYView; y < endYView; y++)
		for (int x = startXView; x < endXView; x++)
		{
			auto& b = gameData.gameMap.getBlockUnsafe(x, y);

			if (b.type != Block::air)
			{

				DrawTexturePro(
					assetManager.textures, 
					getTextureAtlas(b.type, 0, 32, 32), // source
					{(float)x, (float)y, 1, 1}, // dest
					{0, 0}, // origin (top-left corner)
					0.0f, // rotation
					WHITE // tint
				);

			}

		}

	// Draw select block
	DrawTexturePro(
		assetManager.frame,
		{ 0.f ,0.f, (float)assetManager.frame.width, (float)assetManager.frame.height }, // source
		{ (float)blockX, (float)blockY, 1, 1 }, // dest
		{ 0, 0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);

#pragma endregion

	EndMode2D();

	DrawFPS(10, 10);

	return !IsKeyDown(KEY_ESCAPE);
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
	//std::ofstream f(RESOURCES_PATH "f.txt");
	//f << "\nCLOSED\n";
	//f.close();
}