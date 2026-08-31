#pragma once
#include "Component.h"
#include <functional>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

class RespawnComponent : public IComponent
{
public:
    RespawnComponent(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

    void StartRespawn();

private:
    bool isWaitingForRespawn = false;
    float respawnTime = 5.0f;
    float timer = 0.0f;

    glm::vec3 minSpawnArea{ 0.0f };
    glm::vec3 maxSpawnArea{ 0.0f };

    void DoRespawn();
};