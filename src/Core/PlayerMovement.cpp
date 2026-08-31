#include "PlayerMovement.h"
#include "GameObject.h"
#include "ComponentRegistry.h"
#include "Transform.h"
#include "../InputManager.h"
#include "../Rendering/SpriteRenderer.h"
#include "AnimationComponent.h"
#include <glm/glm.hpp>

REGISTER_COMPONENT(PlayerMovement);

PlayerMovement::PlayerMovement(GameObject* owner)
    : IComponent(owner)
{
}

void PlayerMovement::Update(float deltaTime)
{
    auto anim = owner->GetComponent<AnimationComponent>();
    auto sprite = owner->GetComponent<SpriteRenderer>();
    if (!anim || !sprite) return;

    if (anim->GetCurrentAnimationName() == "death")
        return;

    glm::vec3 movement(0.0f);

    if (InputManager::Instance().IsKeyPressed(InputManager::Key::W))
        movement.y += speed * deltaTime;
    if (InputManager::Instance().IsKeyPressed(InputManager::Key::S))
        movement.y -= speed * deltaTime;
    if (InputManager::Instance().IsKeyPressed(InputManager::Key::A))
        movement.x -= speed * deltaTime;
    if (InputManager::Instance().IsKeyPressed(InputManager::Key::D))
        movement.x += speed * deltaTime;

    owner->GetTransform().Position += movement;

    if (!anim->IsLocked())
    {
        if (movement.x > 0) {
            facing = FacingDirection::Right;
            anim->Play("walk_right");
            sprite->SetFlipX(false);
        }
        else if (movement.x < 0) {
            facing = FacingDirection::Left;
            anim->Play("walk_right");
            sprite->SetFlipX(true);
        }
        else if (movement.y > 0) {
            facing = FacingDirection::Up;
            anim->Play("walk_up");
            sprite->SetFlipX(false);
        }
        else if (movement.y < 0) {
            facing = FacingDirection::Down;
            anim->Play("walk_down");
            sprite->SetFlipX(false);
        }
        else {
            switch (facing) {
            case FacingDirection::Right:
                anim->Play("idle_right");
                sprite->SetFlipX(false);
                break;
            case FacingDirection::Left:
                anim->Play("idle_right");
                sprite->SetFlipX(true);
                break;
            case FacingDirection::Up:
                anim->Play("idle_down");
                break;
            case FacingDirection::Down:
                anim->Play("idle");
                break;
            }
        }
    }
}

void PlayerMovement::Deserialize(const nlohmann::json& data)
{
    if (data.contains("speed"))
        speed = data["speed"].get<float>();
}