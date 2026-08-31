#include "RespawnComponent.h"
#include "GameObject.h"
#include "PickupComponent.h"
#include "EnemyHealthComponent.h"
#include "ComponentRegistry.h"

#include <SDL3/SDL.h>
#include <random>

REGISTER_COMPONENT(RespawnComponent);

RespawnComponent::RespawnComponent(GameObject* owner)
    : IComponent(owner)
{
}

void RespawnComponent::Deserialize(const nlohmann::json& data)
{
    if (data.contains("respawnTime"))
        respawnTime = data["respawnTime"].get<float>();

    if (data.contains("minSpawnArea"))
    {
        auto a = data["minSpawnArea"];
        minSpawnArea = glm::vec3(a[0], a[1], a[2]);
    }

    if (data.contains("maxSpawnArea"))
    {
        auto a = data["maxSpawnArea"];
        maxSpawnArea = glm::vec3(a[0], a[1], a[2]);
    }
}

void RespawnComponent::Update(float deltaTime)
{
    if (!isWaitingForRespawn)
        return;

    timer += deltaTime;

    if (timer >= respawnTime)
    {
        timer = 0.0f;
        isWaitingForRespawn = false;
        DoRespawn();
    }
}

void RespawnComponent::StartRespawn()
{
    if (isWaitingForRespawn)
        return;

    isWaitingForRespawn = true;
    timer = 0.0f;
}

void RespawnComponent::DoRespawn()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> x(minSpawnArea.x, maxSpawnArea.x);
    std::uniform_real_distribution<float> y(minSpawnArea.y, maxSpawnArea.y);
    std::uniform_real_distribution<float> z(minSpawnArea.z, maxSpawnArea.z);

    glm::vec3 newPos(x(gen), y(gen), z(gen));

    owner->GetTransform().Position = newPos;
    owner->GetTransform().Scale = glm::vec3(1.0f);

    if (auto* health = owner->GetComponent<EnemyHealthComponent>())
    {
        health->ResetHP();
    }

    if (auto* pickup = owner->GetComponent<PickupComponent>())
    {
        pickup->Reset();
    }

    SDL_Log("Respawned %s at (%.1f, %.1f, %.1f)",
        owner->GetName().c_str(),
        newPos.x, newPos.y, newPos.z);
}