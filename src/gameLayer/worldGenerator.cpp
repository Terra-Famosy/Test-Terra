#include "worldGenerator.h"
#include "randomStuff.h"
#include <FastNoiseSIMD.h>



float lerp(float a, float b, float t); // forward declaration

void generateWorld(GameMap& gameMap, int seed)
{
	const int w = 900;
	const int h = 500;

	gameMap.create(w, h);

	std::ranlux24_base rng(seed++);

#pragma region Noise


	std::unique_ptr<FastNoiseSIMD> dirtNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> stoneNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> regionNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> caveNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirtNoiseGenerator->SetSeed(seed++);
	stoneNoiseGenerator->SetSeed(seed++);
	regionNoiseGenerator->SetSeed(seed++);
	caveNoiseGenerator->SetSeed(seed++);

	dirtNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirtNoiseGenerator->SetFractalOctaves(6);
	dirtNoiseGenerator->SetFrequency(0.02f);

	stoneNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	stoneNoiseGenerator->SetFractalOctaves(4);
	stoneNoiseGenerator->SetFrequency(0.01f);

	regionNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	regionNoiseGenerator->SetFractalOctaves(6);
	regionNoiseGenerator->SetFractalGain(0.4f);
	regionNoiseGenerator->SetFrequency(0.02f);

	caveNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	caveNoiseGenerator->SetFractalOctaves(3);
	caveNoiseGenerator->SetFrequency(0.02f);

	float *dirtNoise = FastNoiseSIMD::GetEmptySet(w);
	float *stoneNoise = FastNoiseSIMD::GetEmptySet(w);
	float *regionNoise = FastNoiseSIMD::GetEmptySet(w);

	dirtNoiseGenerator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
	stoneNoiseGenerator->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);
	regionNoiseGenerator->FillNoiseSet(regionNoise, 0, 0, 0, w, 1, 1);

	for (int i = 0; i < w; i++)
	{
		dirtNoise[i] = (dirtNoise[i] + 1) / 2; //normalize to 0-1
		stoneNoise[i] = (stoneNoise[i] + 1) / 2; //normalize to 0-1
		regionNoise[i] = (regionNoise[i] + 1) / 2; //normalize to 0-1
	}

	float *caveNoise = FastNoiseSIMD::GetEmptySet(w * h);
	caveNoiseGenerator->FillNoiseSet(caveNoise, 0, 0, 0, h, w, 1);

	for (int i = 0; i < w * h; i++)
	{
		caveNoise[i] = (caveNoise[i] + 1) / 2; //normalize to 0-1
	}

	auto getCaveNoise = [&](int x, int y) -> float
	{
		return caveNoise[y * w + x];
	};

	int dirtOffsetStart = -5;
	int dirtOffsetEnd = 35;

	int stoneOffsetStart = 80;
	int stoneOffsetEnd = 170;

	for (int x = 0; x < w; x++)
	{
		int stoneHeight = lerp(stoneOffsetStart, stoneOffsetEnd, stoneNoise[x]);
		int dirtHeight = lerp(dirtOffsetStart, dirtOffsetEnd, dirtNoise[x]);
		dirtHeight = stoneHeight - dirtHeight;

		for (int y = 0; y < h; y++)
		{
			Block b;
			if (y > dirtHeight)
			{
				b.type = Block::dirt;
			}

			if (y == dirtHeight)
			{
				b.type = Block::grassBlock;
			}

			if (y >= stoneHeight)
			{
				b.type = Block::stone;
			}

			if (getCaveNoise(x, y) < 0.80f && getCaveNoise(x, y) > 0.60f)
			{
				b.type = Block::air;
			}

			gameMap.getBlockUnsafe(x, y) = b;
		}
	}

#pragma endregion

#pragma region Worldgen without noise
	//int keepDirectionTimeDirt = getRandomInt(rng, 5, 20);
	//int directionDirt = getRandomInt(rng, -2, 2);

	//int keepDirectionTimeStone = getRandomInt(rng, 5, 20);
	//int directionStone = getRandomInt(rng, -2, 2);

	//int dirtHeight = 70;
	//int stoneHeight = 90;

	//for (int x = 0; x < w; x++)
	//{

	//	keepDirectionTimeDirt--;
	//	if (keepDirectionTimeDirt <= 0)
	//	{
	//		keepDirectionTimeDirt = getRandomInt(rng, 5, 20);
	//		directionDirt = getRandomInt(rng, -2, 2);
	//	}

	//	if (directionDirt == -1)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			dirtHeight--;
	//		}
	//	}
	//	else if (directionDirt == -2)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			dirtHeight--;
	//		}

	//		if (getRandomChance(rng, 0.25))
	//		{
	//			dirtHeight--;
	//		}
	//	}
	//	else if (directionDirt == 1)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			dirtHeight++;
	//		}
	//	}
	//	else if (directionDirt == 2)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			dirtHeight++;
	//		}

	//		if (getRandomChance(rng, 0.25))
	//		{
	//			dirtHeight++;
	//		}
	//	}

	//	if (dirtHeight < 50)
	//	{
	//		dirtHeight = 50;
	//	}

	//	if (dirtHeight > 90)
	//	{
	//		dirtHeight = 90;
	//	}

	//	//same code for stone
	//	keepDirectionTimeStone--;
	//	if (keepDirectionTimeStone <= 0)
	//	{
	//		keepDirectionTimeStone = getRandomInt(rng, 5, 20);
	//		directionStone = getRandomInt(rng, -2, 2);
	//	}

	//	if (directionStone == -1)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			stoneHeight--;
	//		}
	//	}
	//	else if (directionStone == -2)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			stoneHeight--;
	//		}

	//		if (getRandomChance(rng, 0.25))
	//		{
	//			stoneHeight--;
	//		}
	//	}
	//	else if (directionStone == 1)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			stoneHeight++;
	//		}
	//	}
	//	else if (directionStone == 2)
	//	{
	//		if (getRandomChance(rng, 0.25))
	//		{
	//			stoneHeight++;
	//		}

	//		if (getRandomChance(rng, 0.25))
	//		{
	//			stoneHeight++;
	//		}
	//	}

	//	if (stoneHeight < 60)
	//	{
	//		stoneHeight = 60;
	//	}

	//	if (stoneHeight > 120)
	//	{
	//		stoneHeight = 120;
	//	}

	//	for (int y = 0; y < h; y++)
	//	{
	//		Block b;

	//		if (y > dirtHeight)
	//		{
	//			b.type = Block::dirt;
	//		}

	//		if (y == dirtHeight)
	//		{
	//			b.type = Block::grassBlock;
	//		}

	//		if (y > stoneHeight)
	//		{
	//			b.type = Block::stone;
	//		}

	//		gameMap.getBlockUnsafe(x, y) = b;

	//	}

	//}
#pragma endregion

	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	FastNoiseSIMD::FreeNoiseSet(stoneNoise);
	FastNoiseSIMD::FreeNoiseSet(regionNoise);
	FastNoiseSIMD::FreeNoiseSet(caveNoise);

}

float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}