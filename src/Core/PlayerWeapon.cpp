#include "PlayerWeapon.h"
#include "GameObject.h"
#include "Scene.h"
#include "EnemyHealthComponent.h"
#include "Transform.h"
#include "../InputManager.h"
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include "ComponentRegistry.h"
#include "AnimationComponent.h"
#include "../Rendering/SpriteRenderer.h"

REGISTER_COMPONENT(PlayerWeapon);

PlayerWeapon::PlayerWeapon(GameObject* owner)
    : IComponent(owner), attackTimer(0.0f)
{

}

void PlayerWeapon::Update(float deltaTime)
{
    attackTimer += deltaTime;

    if (!anim)
        anim = owner->GetComponent<AnimationComponent>();
    if (!anim) return;

    if (anim->GetCurrentAnimationName() == "death")
        return;

    if (InputManager::Instance().IsMousePressed(InputManager::MouseButton::Left) &&
        attackTimer >= attackInterval)
    {
        anim->Play("attack_right", true);
        DealDamage();
        attackTimer = 0.0f;
    }
}

void PlayerWeapon::DealDamage()
{
    Scene* scene = owner->GetScene();
    if (!scene) return;

    glm::vec2 myPos(owner->GetTransform().Position.x, owner->GetTransform().Position.y);

    for (auto& obj : scene->GetGameObjects())
    {
        if (obj.get() == owner) continue;

        EnemyHealthComponent* health = obj->GetComponent<EnemyHealthComponent>();
        if (!health || health->GetCurrentHP() <= 0.0f)
            continue;

        glm::vec2 targetPos(obj->GetTransform().Position.x, obj->GetTransform().Position.y);
        float distance = glm::length(targetPos - myPos);

        if (distance <= attackRadius)
        {
            SDL_Log("Player weapon hit: %s for %.2f damage",
                obj->GetName().c_str(), damage);
            health->TakeDamage(damage);
        }
    }
}

void PlayerWeapon::Deserialize(const nlohmann::json& data)
{
    if (data.contains("attackRadius"))
        attackRadius = data["attackRadius"].get<float>();

    if (data.contains("damage"))
        damage = data["damage"].get<float>();

    if (data.contains("attackInterval"))
        attackInterval = data["attackInterval"].get<float>();
}