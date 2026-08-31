#include "AnimationComponent.h"
#include "../Rendering/SpriteRenderer.h"
#include "GameObject.h"
#include "ComponentRegistry.h"
#include <SDL3/SDL.h>

REGISTER_COMPONENT(AnimationComponent);

AnimationComponent::AnimationComponent(GameObject* owner)
    : IComponent(owner)
{
}

void AnimationComponent::Deserialize(const nlohmann::json& data)
{
    sprite = owner->GetComponent<SpriteRenderer>();
    if (!sprite) return;

    if (data.contains("animations"))
    {
        for (auto& [key, value] : data["animations"].items())
        {
            Animation anim;
            anim.startFrame = value.value("startFrame", 0);
            anim.frameCount = value.value("frameCount", 1);
            anim.frameTime = value.value("frameTime", 0.2f);
            anim.row = value.value("row", 0);
            anim.loop = value.value("loop", true);
            animations[key] = anim;
        }
    }

    if (sprite->GetTextureWidth() > 0 && sprite->GetTextureHeight() > 0)
        Play("idle");
}

void AnimationComponent::Play(const std::string& name, bool force)
{
    if (currentAnimationName == "death")
        return;

    if (!force && currentAnimationName == name)
        return;

    auto it = animations.find(name);
    if (it == animations.end())
        return;

    currentAnimation = &it->second;
    currentAnimationName = name;
    currentFrame = 0;
    timer = 0.0f;

    locked = !currentAnimation->loop;
    UpdateUV();
}

void AnimationComponent::Update(float deltaTime)
{
    if (!sprite || !currentAnimation) return;

    timer += deltaTime;
    if (timer >= currentAnimation->frameTime)
    {
        timer = 0.0f;
        currentFrame++;

        if (currentFrame >= currentAnimation->frameCount)
        {
            if (currentAnimation->loop)
                currentFrame = 0;
            else
            {
                currentFrame = currentAnimation->frameCount - 1;
                locked = false;
                return;
            }
        }

        UpdateUV();
    }
}

void AnimationComponent::UpdateUV()
{
    if (!sprite || !currentAnimation) return;
    if (sprite->GetTextureWidth() == 0 || sprite->GetTextureHeight() == 0) return;

    float sheetWidth = static_cast<float>(sprite->GetTextureWidth());
    float sheetHeight = static_cast<float>(sprite->GetTextureHeight());

    constexpr float frameWidth = 32.0f;
    constexpr float frameHeight = 32.0f;

    int frameIndex = currentFrame;
    int row = currentAnimation->row;

    glm::vec2 scale(frameWidth / sheetWidth, frameHeight / sheetHeight);
    glm::vec2 offset;
    offset.x = frameIndex * scale.x;
    offset.y = 1.0f - scale.y - row * scale.y;

    sprite->SetTextureScale(scale);
    sprite->SetTextureOffset(offset);
}