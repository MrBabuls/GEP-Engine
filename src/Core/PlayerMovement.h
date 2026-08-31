#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

enum class FacingDirection { Right, Left, Up, Down };

class PlayerMovement : public IComponent
{
public:
    PlayerMovement(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

private:
    float speed;
    FacingDirection facing = FacingDirection::Right;
};