#pragma once
#include "Component.h"
#include <string>
#include <nlohmann/json.hpp>

class SpriteRenderer;

class UIButtonComponent : public IComponent
{
public:
    UIButtonComponent(GameObject* owner);

    void Update(float dt) override;
    void Deserialize(const nlohmann::json& data) override;

private:
    std::string targetScene;
    bool wasPressed = false;

    bool IsMouseOver() const;
};