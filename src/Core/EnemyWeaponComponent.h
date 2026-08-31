#pragma once
#include "Component.h"
#include <functional>
#include <nlohmann/json.hpp>

class AnimationComponent;

class EnemyWeaponComponent : public IComponent
{
public:
    EnemyWeaponComponent(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

    void DealDamage();

    void SetOnHitCallback(std::function<void(GameObject*)> callback) { onHit = callback; }

private:
    float attackRadius = 50.0f;
    float damage = 10.0f;
    float attackInterval = 1.0f;
    float attackTimer = 0.0f;

    std::function<void(GameObject*)> onHit;

    AnimationComponent* anim = nullptr;
};
