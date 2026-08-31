#pragma once
#include "Component.h"
#include "../Rendering/SpriteRenderer.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>

struct Animation
{
    int startFrame = 0;
    int frameCount = 1;
    float frameTime = 0.2f;
    int row = 0;
    bool loop = true;
};

class AnimationComponent : public IComponent
{
public:
    AnimationComponent(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;
    void Play(const std::string& name, bool force = false);

    std::string GetCurrentAnimationName() const { return currentAnimationName; }

    bool IsLocked() const { return locked; }

private:
    void UpdateUV();

    SpriteRenderer* sprite = nullptr;

    Animation* currentAnimation = nullptr;
    std::string currentAnimationName;
    std::unordered_map<std::string, Animation> animations;

    int currentFrame;
    float timer = 0.0f;
    bool locked = false;
};