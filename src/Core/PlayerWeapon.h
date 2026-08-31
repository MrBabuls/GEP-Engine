#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <functional>

class AnimationComponent;

class PlayerWeapon : public IComponent
{
public:
    PlayerWeapon(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

    void SetOnHitCallback(std::function<void(GameObject*)> callback) { onHit = callback; }
    void DealDamage();

private:
    float attackRadius = 100.0f;
    float damage = 10.0f;
    float attackInterval = 1.0f;
    float attackTimer = 0.0f;

    std::function<void(GameObject*)> onHit;

    AnimationComponent* anim = nullptr;
};
