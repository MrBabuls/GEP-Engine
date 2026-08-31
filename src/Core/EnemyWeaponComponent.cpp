#include "EnemyWeaponComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "PlayerHealthComponent.h"
#include "Transform.h"
#include "EnemyHealthComponent.h"
#include "AnimationComponent.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "ComponentRegistry.h"

REGISTER_COMPONENT(EnemyWeaponComponent);

EnemyWeaponComponent::EnemyWeaponComponent(GameObject* owner)
    : IComponent(owner)
{
}

void EnemyWeaponComponent::Deserialize(const nlohmann::json& data)
{
    if (data.contains("attackRadius"))
        attackRadius = data["attackRadius"].get<float>();
    if (data.contains("damage"))
        damage = data["damage"].get<float>();
    if (data.contains("attackInterval"))
        attackInterval = data["attackInterval"].get<float>();
}

void EnemyWeaponComponent::Update(float deltaTime)
{
    attackTimer += deltaTime;

    auto anim = owner->GetComponent<AnimationComponent>();
    if (!anim) return;

    if (anim->GetCurrentAnimationName() == "death")
        return;

    Scene* scene = owner->GetScene();
    if (!scene || !scene->IsPlayerAlive())
        return;

    GameObject* player = scene->FindObjectByName("Player");
    if (!player) return;

    glm::vec2 myPos(owner->GetTransform().Position.x, owner->GetTransform().Position.y);
    glm::vec2 playerPos(player->GetTransform().Position.x, player->GetTransform().Position.y);
    float distance = glm::length(playerPos - myPos);

    if (distance <= attackRadius && attackTimer >= attackInterval)
    {
        anim->Play("attack_right", true);
        DealDamage();
        attackTimer = 0.0f;
    }
}

void EnemyWeaponComponent::DealDamage()
{
    Scene* scene = owner->GetScene();
    if (!scene) return;

    if (!scene->IsPlayerAlive())
        return;

    EnemyHealthComponent* healthComp = owner->GetComponent<EnemyHealthComponent>();
    if (healthComp && healthComp->GetCurrentHP() <= 0.0f)
        return;

    glm::vec2 myPos(owner->GetTransform().Position.x, owner->GetTransform().Position.y);

    for (auto& obj : scene->GetGameObjects())
    {
        if (obj.get() == owner) continue;

        PlayerHealthComponent* playerHealth = obj->GetComponent<PlayerHealthComponent>();
        if (!playerHealth) continue;

        if (playerHealth->GetCurrentHP() <= 0.0f)
            continue;

        glm::vec2 targetPos(obj->GetTransform().Position.x, obj->GetTransform().Position.y);
        float distance = glm::length(targetPos - myPos);

        if (distance <= attackRadius)
        {
            playerHealth->TakeDamage(damage);
            SDL_Log("Enemy hit Player for %.2f damage", damage);
        }
    }
}