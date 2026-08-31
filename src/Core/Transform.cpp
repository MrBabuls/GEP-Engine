#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transform::GetMatrix() const 
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
    glm::mat4 rotationMatrix = glm::mat4_cast(Rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}