#pragma once
#include "Component.h"
#include "../Rendering/SpriteRenderer.h"
#include <glm/glm.hpp>
#include <functional>
#include <nlohmann/json.hpp>

class PlayerHealthComponent;

class PickupComponent : public IComponent
{
public:
    PickupComponent(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

    void SetOnCollectedCallback(std::function<void()> callback) { onCollected = callback; }

    void Reset();

private:
    float radius = 40.0f;
    bool collected = false;
    glm::vec3 originalPosition;

    std::function<void()> onCollected;

    bool CheckCollision(GameObject* player);

    SpriteRenderer* sprite = nullptr;
};
