#pragma once
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "Rendering/SpriteRenderer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Core/Scene.h"
#include "Core/ResourceManager.h"
#include <memory>

class Engine {
public:
    Engine();
    ~Engine();

    bool initialize(const char* title, int width, int height);
    void run();
    void clean();

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    bool m_isRunning = false;

    std::shared_ptr<Shader> m_spriteShader;
    std::shared_ptr<Texture> m_playerTexture;
    std::shared_ptr<Texture> m_enemyTexture;
    std::shared_ptr<Texture> m_pickupTexture;
    std::shared_ptr<Texture> m_playButtonTexture;
    std::shared_ptr<Texture> m_youDiedTexture;
    std::shared_ptr<Texture> m_returnButtonTexture;
};