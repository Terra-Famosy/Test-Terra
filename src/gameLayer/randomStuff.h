#pragma once
#include <random>

// Returns a float in [min, Max]
float getRandomFloat(std::ranlux24_base& rng, float min, float max);

// Returns a int in [min, Max]
int getRandomInt(std::ranlux24_base& rng, float min, float max);


bool getRandomChance(std::ranlux24_base& rng, float chance);