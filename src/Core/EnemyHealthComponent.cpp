#include "EnemyHealthComponent.h"
#include "GameObject.h"
#include "ComponentRegistry.h"
#include "RespawnComponent.h"
#include <SDL3/SDL.h>

REGISTER_COMPONENT(EnemyHealthComponent);

EnemyHealthComponent::EnemyHealthComponent(GameObject* owner)
    : IComponent(owner)
{
    currentHP = maxHP;
}

void EnemyHealthComponent::Update(float)
{

}

void EnemyHealthComponent::TakeDamage(float amount)
{
    currentHP -= amount;
    SDL_Log("%s took %.2f damage, HP: %.2f", owner->GetName().c_str(), amount, currentHP);

    if (currentHP <= 0.0f)
    {
        SDL_Log("[DEBUG] EnemyHealthComponent: %s died", owner->GetName().c_str());

        owner->GetTransform().Scale = glm::vec3(0.0f);

        if (auto* respawn = owner->GetComponent<RespawnComponent>())
        {
            respawn->StartRespawn();
        }
    }
}

void EnemyHealthComponent::Deserialize(const nlohmann::json& data)
{
    if (data.contains("maxHP"))
        maxHP = data["maxHP"].get<float>();
}