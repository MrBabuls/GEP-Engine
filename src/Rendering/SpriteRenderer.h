#pragma once
#include "../Core/Component.h"
#include "../Rendering/Texture.h"
#include "../Core/ResourceManager.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>

class SpriteRenderer : public IDrawableComponent
{
public:
    SpriteRenderer(GameObject* owner);
    ~SpriteRenderer();

    void SetTexture(std::shared_ptr<Texture> tex);

    void SetColor(const glm::vec3& col) { color = col; }
    void SetSize(const glm::vec2& s) { size = s; }
    glm::vec2 GetSize() const { return size; }
    void SetLayer(float l) { layer = l; }

    void Draw(const Camera& camera) const override;
    void Deserialize(const nlohmann::json& serializedData) override;

    int GetTextureWidth() const { return textureWidth; }
    int GetTextureHeight() const { return textureHeight; }

    void SetTextureOffset(const glm::vec2& offset) { uvOffset = offset; }
    void SetTextureScale(const glm::vec2& scale) { uvScale = scale; }

    void SetFlipX(bool flip) { flipX = flip; }
    bool IsFlipX() const { return flipX; }

private:
    void SetupQuad();

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    float layer = 0.0f;
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec2 size = glm::vec2(1.0f);

    glm::vec2 uvOffset = glm::vec2(0.0f);
    glm::vec2 uvScale = glm::vec2(1.0f);

    bool flipX = false;

    unsigned int textureID = 0;
    std::shared_ptr<Texture> texture = nullptr;
    int textureWidth = 0;
    int textureHeight = 0;

    // Quad vertices ja indices
    static inline float quadVerts[16] = {
        1,  1, 1, 1,
        1, -1, 1, 0,
       -1, -1, 0, 0,
       -1,  1, 0, 1
    };

    static inline unsigned int quadIndices[6] = { 0, 1, 3, 1, 2, 3 };
};