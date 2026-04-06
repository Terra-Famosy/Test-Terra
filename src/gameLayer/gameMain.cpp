#include <iostream>
#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>
#include <randomStuff.h>
#include <worldGenerator.h>
#include <imgui.h>

struct GameData
{
	GameMap gameMap;
	GameMap backgroundGameMap;
	Camera2D camrea;
	int seed = 12345;

}gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap, gameData.seed);

	gameData.backgroundGameMap = gameData.gameMap;

	gameData.camrea.target = { 20, 120 }; // world-space center of view
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
	static float CAMERA_SPEED = 10.f;
	if (IsKeyDown(KEY_A)) gameData.camrea.target.x -= CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_D)) gameData.camrea.target.x += CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_W)) gameData.camrea.target.y -= CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_S)) gameData.camrea.target.y += CAMERA_SPEED * deltaTime;
#pragma endregion



	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camrea);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	static Block block{};

	block.type -= (int)GetMouseWheelMove();

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
			b->type = block.type;
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
				if (b.type == Block::woodLog)
				{
					auto* bUP = gameData.gameMap.getBlockSafe(x, y - 1);
					auto* bDOWN = gameData.gameMap.getBlockSafe(x, y + 1);
					auto* bLEFT = gameData.gameMap.getBlockSafe(x - 1, y);
					auto* bRIGHT = gameData.gameMap.getBlockSafe(x + 1, y);

					int blockState = 0;

					// Prüfen auf nullptr und dann mit -> auf Feld zugreifen
					auto isType = [](const Block* b, auto t) { return b && b->type == t; };

					bool upIsLog = isType(bUP, Block::woodLog);
					bool downIsLog = isType(bDOWN, Block::woodLog);
					bool downIsSolid = bDOWN && bDOWN->type != Block::air && bDOWN->type != Block::woodLog;
					bool leftIsLeaves = isType(bLEFT, Block::leaves);
					bool rightIsLeaves = isType(bRIGHT, Block::leaves);
					bool upIsLeaves = isType(bUP, Block::leaves);

					if (downIsLog && !upIsLog) // log with down log
					{
						if (upIsLeaves || rightIsLeaves || leftIsLeaves)
							blockState = 5;
						else
							blockState = 6;
					}
					else if (downIsSolid) // supported by non-air/non-log block below
					{
						if (upIsLog)
							blockState = 4;
						else
							blockState = 7;
					}
					else
					{
						if (!leftIsLeaves && !rightIsLeaves) blockState = 0; // log
						else if (leftIsLeaves && rightIsLeaves) blockState = 1; // log with leaves on both sides
						else if (rightIsLeaves) blockState = 2; // log with leaves on left
						else if (leftIsLeaves) blockState = 3; // log with leaves on right
					}

					DrawTexturePro(
						assetManager.woodLog,
						getTextureAtlas(blockState, 0, 32, 32), // source
						{ (float)x, (float)y, 1, 1 }, // dest
						{ 0, 0 }, // origin (top-left corner)
						0.0f, // rotation
						WHITE // tint
					);
				}
				else
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

		}

	// Draw selected Block
	DrawTexturePro(
		assetManager.textures,
		getTextureAtlas(block.type, 0, 32, 32), // source
		{ (float)bottomRightView.x - 1, (float)topLeftView.y, 1, 1 }, // dest
		{ 0, 0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);

	// Draw select block
	DrawTexturePro(
		assetManager.frame,
		{ 0.f ,0.f, (float)assetManager.frame.width, (float)assetManager.frame.height }, // source
		{ (float)blockX, (float)blockY, 1, 1 }, // dest
		{ 0, 0 }, // origin (top-left corner)
		0.0f, // rotation
		WHITE // tint
	);

	EndMode2D();

	ImGui::Begin("Game controll");

	ImGui::SliderFloat("Camera zoom", &gameData.camrea.zoom, 10.f, 150.f);
	ImGui::SliderFloat("Camera speed", &CAMERA_SPEED, 5.f, 30.f);
	ImGui::SliderInt("Seed", &gameData.seed, 0, 10000);
	if (ImGui::Button("Regenerate world"))
	{
		generateWorld(gameData.gameMap, gameData.seed);
	}

	ImGui::End();

#pragma endregion

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