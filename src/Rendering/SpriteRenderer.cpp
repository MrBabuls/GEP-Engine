#include "SpriteRenderer.h"
#include "../Core/GameObject.h"
#include "../Rendering/Texture.h"
#include "../Core/ResourceManager.h"
#include "../Core/ComponentRegistry.h"
#include "../Rendering/Camera.h"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

REGISTER_COMPONENT(SpriteRenderer);

SpriteRenderer::SpriteRenderer(GameObject* owner)
    : IDrawableComponent(owner)
{
    SetupQuad();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void SpriteRenderer::SetTexture(std::shared_ptr<Texture> tex)
{
    texture = tex;
    if (tex) {
        textureID = tex->GetId();
        textureWidth = tex->GetWidth();
        textureHeight = tex->GetHeight();
    }
}

void SpriteRenderer::SetupQuad()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void SpriteRenderer::Draw(const Camera& camera) const
{
    if (!owner) return;

    auto shader = ResourceManager::Instance().GetShader("texture_shader");
    if (!shader)
    {
        SDL_Log("[Draw] Shader not found!");
        return;
    }

    shader->Use();

    shader->SetBool("useTexture", textureID != 0);
    shader->SetBool("flipX", flipX);

    glm::vec3 pos = owner->GetTransform().Position;
    glm::quat rot = owner->GetTransform().Rotation;
    glm::vec3 scale = owner->GetTransform().Scale;

    glm::vec3 scale3D = glm::vec3(size, 1.0f) * scale;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model *= glm::mat4_cast(rot);
    model = glm::scale(model, scale3D);

    shader->SetMat4("model", model);
    shader->SetMat4("view", camera.GetViewMatrix());
    shader->SetMat4("projection", camera.GetProjectionMatrix());
    shader->SetFloat("layer", layer);
    shader->SetVec3("spriteColor", color);
    shader->SetVec2("uvOffset", uvOffset);
    shader->SetVec2("uvScale", uvScale);

    if (textureID != 0)
        glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::Deserialize(const nlohmann::json& serializedData)
{
    if (serializedData.contains("texture"))
    {
        auto textureId = serializedData["texture"].get<std::string>();
        SetTexture(ResourceManager::Instance().GetTexture(textureId));
    }

    if (serializedData.contains("color"))
    {
        auto& colorData = serializedData["color"];
        SetColor(glm::vec3(
            colorData[0].get<float>() / 255.f,
            colorData[1].get<float>() / 255.f,
            colorData[2].get<float>() / 255.f));
    }

    if (serializedData.contains("size"))
    {
        auto& sizeData = serializedData["size"];
        SetSize(glm::vec2(
            sizeData[0].get<float>(),
            sizeData[1].get<float>()));
    }
}