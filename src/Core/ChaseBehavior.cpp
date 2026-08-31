#include "ChaseBehavior.h"
#include "GameObject.h"
#include "ComponentRegistry.h"
#include "Scene.h"
#include "Transform.h"
#include "AnimationComponent.h"
#include "../Rendering/SpriteRenderer.h"
#include <glm/glm.hpp>


REGISTER_COMPONENT(ChaseBehavior);

ChaseBehavior::ChaseBehavior(GameObject* owner)
    : IComponent(owner), speed(100.0f), chaseRadius(300.0f)
{
}

void ChaseBehavior::Update(float deltaTime)
{
    auto* anim = owner->GetComponent<AnimationComponent>();
    auto* sprite = owner->GetComponent<SpriteRenderer>();
    if (!anim || !sprite) return;

    Scene* scene = owner->GetScene();
    if (!scene) return;

    GameObject* player = scene->FindObjectByName("Player");
    if (!player) return;

    glm::vec2 myPos(owner->GetTransform().Position.x, owner->GetTransform().Position.y);
    glm::vec2 playerPos(player->GetTransform().Position.x, player->GetTransform().Position.y);

    glm::vec2 delta = playerPos - myPos;
    float distance = glm::length(delta);

    const float minDistance = 50.0f;

    if (anim->GetCurrentAnimationName() == "death")
        return;

    if (distance <= chaseRadius && distance > minDistance)
    {
        glm::vec2 direction = glm::normalize(delta);
        glm::vec2 move = direction * speed * deltaTime;

        if (glm::length(move) > (distance - minDistance))
            move = direction * (distance - minDistance);

        owner->GetTransform().Position.x += move.x;
        owner->GetTransform().Position.y += move.y;

        if (!anim->IsLocked())
        {
            if (glm::abs(delta.x) > glm::abs(delta.y))
            {
                anim->Play("walk_right", false);
                sprite->SetFlipX(delta.x < 0);
            }
            else
            {
                anim->Play(delta.y > 0 ? "walk_up" : "walk_down", false);
                sprite->SetFlipX(false);
            }
        }
        else if (distance > chaseRadius)
        {
            anim->Play("idle", false);
        }
    }
}

void ChaseBehavior::Deserialize(const nlohmann::json& data)
{
    if (data.contains("speed"))
        speed = data["speed"].get<float>();

    if (data.contains("chaseRadius"))
        chaseRadius = data["chaseRadius"].get<float>();
}