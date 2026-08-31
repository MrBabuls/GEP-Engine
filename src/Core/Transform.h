#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    glm::vec3 Position{ 0.f, 0.f, 0.f };
    glm::quat Rotation{ 1.f, 0.f, 0.f, 0.f };
    glm::vec3 Scale{ 1.f, 1.f, 1.f };

    glm::mat4 GetMatrix() const;
};