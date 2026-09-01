#include "WorldGenerator.h"
#include "../Core/Scene.h"

#include "SDL3/SDL_log.h"
#include <random>

void WorldGenerator::Generate(Scene& scene)
{
	SDL_Log("[WorldGenerator] Generate() called");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_real_distribution<float> positionX(-300.0f, 300.0f);
    std::uniform_real_distribution<float> positionY(-200.0f, 200.0f);

    for (int i = 0; i < 10; ++i)
    {
        float x = positionX(generator);
        float y = positionY(generator);

        SDL_Log("[WorldGenerator] Tree %d generated at position: (%.2f, %.2f)", i + 1, x, y);
    }

    SDL_Log("[WorldGenerator] World generation complete");
}