#pragma once
#include "Component.h"
#include <nlohmann/json.hpp>
#include <functional>

class PlayerHealthComponent : public IComponent
{
public:
    PlayerHealthComponent(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

    void TakeDamage(float amount);
    float GetCurrentHP() const { return currentHP; }
    void SetOnDeathCallback(std::function<void()> callback) { onDeath = callback; }

private:
    float maxHP = 100.0f;
    float currentHP = 100.0f;
    float deathTimer = 0.0f;
    float timeToQuit = 3.0f;
    bool isDead = false;

    std::function<void()> onDeath;
};