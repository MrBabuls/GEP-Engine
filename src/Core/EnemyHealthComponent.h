#pragma once
#include "Component.h"
#include <nlohmann/json.hpp>
#include <functional>

class EnemyHealthComponent : public IComponent
{
public:
    EnemyHealthComponent(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

    void TakeDamage(float amount);
    float GetCurrentHP() const { return currentHP; }

    void SetOnDeathCallback(std::function<void()> callback) { onDeath = callback; }
    void ResetHP() { currentHP = maxHP; }

private:
    float maxHP = 100.0f;
    float currentHP = 100.0f;
    std::function<void()> onDeath;
};