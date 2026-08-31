#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

class ChaseBehavior : public IComponent 
{
public:
    ChaseBehavior(GameObject* owner);

    void Update(float deltaTime) override;
    void Deserialize(const nlohmann::json& data) override;

private:
    float speed;
    float chaseRadius;
};