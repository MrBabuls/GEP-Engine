#include "Rectangle.h"
#include "../Rendering/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor
Rectangle::Rectangle(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices), vao(0), vbo(0), ebo(0),
    shader(nullptr), texture1(nullptr), texture2(nullptr)
{
    // Initialize VAO, VBO and EBO
    SetupRectangle();
}

// Destructor
Rectangle::~Rectangle()
{
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

void Rectangle::SetShader(const Shader& sh)
{
    shader = &sh;
}

void Rectangle::SetTexture1(const Texture& tex) 
{
    texture1 = &tex;
}

void Rectangle::SetTexture2(const Texture& tex)
{
    texture2 = &tex;
}

void Rectangle::Draw() const
{
    if (!shader)
    {
        std::cerr << "[Rectangle] Shader not set!\n";
        return;
    }

    shader->Use();

    // Bind texture 1
    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetId());
        shader->SetInt("texture1", 0);
    }

    // Bind texture 2
    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetId());
        shader->SetInt("texture2", 1);
    }

    // Send model matrix
    glm::mat4 model = transform.GetMatrix();
    shader->SetMat4("model", model);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


// Setup VAO, VBO, EBO
void Rectangle::SetupRectangle()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates (u, v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}