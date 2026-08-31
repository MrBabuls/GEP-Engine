#include "PlayerHealthComponent.h"
#include "GameObject.h"
#include "../InputManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "AnimationComponent.h"
#include "ComponentRegistry.h"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>

REGISTER_COMPONENT(PlayerHealthComponent);

PlayerHealthComponent::PlayerHealthComponent(GameObject* owner)
    : IComponent(owner)
{
    currentHP = maxHP;
    isDead = false;
    deathTimer = 0.0f;
}

void PlayerHealthComponent::Deserialize(const nlohmann::json& data)
{
    if (data.contains("maxHP"))
        maxHP = data["maxHP"].get<float>();
    currentHP = maxHP;
}

void PlayerHealthComponent::Update(float deltaTime)
{
    if (!isDead) return;

    deathTimer += deltaTime;
    if (deathTimer >= timeToQuit)
    {
        if (owner->GetScene())
        {
            SceneManager::Instance().LoadSceneDeferred("assets/scenes/death.scene");
        }
    }
}

void PlayerHealthComponent::TakeDamage(float amount)
{
    if (isDead) return;

    if (amount < 0.0f)
    {
        currentHP = glm::min(currentHP - amount, maxHP);
        SDL_Log("Player healed %.2f, HP: %.2f", -amount, currentHP);
        return;
    }

    currentHP -= amount;
    SDL_Log("Player took %.2f damage, HP: %.2f", amount, currentHP);

    if (currentHP <= 0.0f)
    {
        isDead = true;
        currentHP = 0.0f;
        SDL_Log("Player died!");

        if (auto* anim = owner->GetComponent<AnimationComponent>())
        {
            anim->Play("death", true);
        }

        InputManager::Instance().DisableAllInput();

        if (owner->GetScene())
            owner->GetScene()->SetPlayerAlive(false);

        deathTimer = 0.0f;
    }

    if (onDeath)
        onDeath();
}