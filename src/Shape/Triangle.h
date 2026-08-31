#pragma once
#include "../Rendering/Shader.h"
#include <vector>
#include <glad/gl.h>
#include <string>

class Triangle {
public:
    Triangle(const std::vector<float>& vertices);
    ~Triangle();

    const void Draw();

private:
    unsigned int vao;
    unsigned int vbo;

    std::vector<float> vertices;
    Shader shader;

    void SetupTriangle();
};