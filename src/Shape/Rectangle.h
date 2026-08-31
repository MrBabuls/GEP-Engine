#pragma once
#include <vector>
#include <glad/gl.h>
#include <string>

#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
#include "../Core/Transform.h"
#include <glm/glm.hpp>

class Rectangle {
public:
    Rectangle(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Rectangle();

    void SetShader(const Shader& shader);
    void SetTexture1(const Texture& tex);
    void SetTexture2(const Texture& tex);
    void Draw() const;

    Transform& GetTransform() { return transform; }

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const Shader* shader = nullptr;
    const Texture* texture1 = nullptr;
    const Texture* texture2 = nullptr;

    Transform transform;

    void SetupRectangle();
};