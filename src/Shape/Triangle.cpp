#include "Triangle.h"
#include "../Rendering/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor
Triangle::Triangle(const std::vector<float>& vertices) 
    : vertices(vertices),
    shader("shaders/vertex.glsl", "shaders/frag.glsl")
{
    // Initialize VAO and VBO
    SetupTriangle();
}

// Destructor
Triangle::~Triangle()
{
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

const void Triangle::Draw()
{
    glUseProgram(shader.GetId());
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

// Setup VAO and VBO
void Triangle::SetupTriangle()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
