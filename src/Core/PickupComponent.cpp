#include "PickupComponent.h"
#include "GameObject.h"
#include "PlayerHealthComponent.h"
#include "Scene.h"
#include "ComponentRegistry.h"
#include "RespawnComponent.h"
#include "Transform.h"

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

REGISTER_COMPONENT(PickupComponent);

PickupComponent::PickupComponent(GameObject* owner)
    : IComponent(owner)
{
    originalPosition = owner->GetTransform().Position;
}

void PickupComponent::Deserialize(const nlohmann::json& data)
{
    if (data.contains("radius"))
        radius = data["radius"].get<float>();

    sprite = owner->GetComponent<SpriteRenderer>();
    if (sprite)
    {
        float sheetW = static_cast<float>(sprite->GetTextureWidth());
        float sheetH = static_cast<float>(sprite->GetTextureHeight());

        constexpr float frameW = 32.0f;
        constexpr float frameH = 32.0f;

        glm::vec2 scale(frameW / sheetW, frameH / sheetH);
        glm::vec2 offset(0.0f, 1.0f - scale.y);

        sprite->SetTextureScale(scale);
        sprite->SetTextureOffset(offset);
    }
}

void PickupComponent::Update(float deltaTime)
{
    if (collected)
        return;

    GameObject* player = owner->GetScene()->FindObjectByName("Player");
    if (!player)
        return;

    if (CheckCollision(player))
    {
        collected = true;

        SDL_Log("[PICKUP] %s collected", owner->GetName().c_str());

        if (auto* health = player->GetComponent<PlayerHealthComponent>())
        {
            health->TakeDamage(-10.0f);
        }

        owner->GetTransform().Scale = glm::vec3(0.0f);

        if (auto* respawn = owner->GetComponent<RespawnComponent>())
        {
            respawn->StartRespawn();
        }

        if (onCollected)
            onCollected();
    }
}

bool PickupComponent::CheckCollision(GameObject* player)
{
    glm::vec2 playerPos(
        player->GetTransform().Position.x,
        player->GetTransform().Position.y
    );

    glm::vec2 myPos(
        owner->GetTransform().Position.x,
        owner->GetTransform().Position.y
    );

    float distance = glm::length(playerPos - myPos);
    return distance <= radius;
}

void PickupComponent::Reset()
{
    collected = false;
    owner->GetTransform().Scale = glm::vec3(1.0f);
}