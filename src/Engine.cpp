#include "Engine.h"
#include "Core/ResourceManager.h"
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "Core/SceneManager.h"
#include "Core/PlayerHealthComponent.h"
#include "Rendering/Camera.h"
#include "Rendering/SpriteRenderer.h"
#include "World/WorldGenerator.h"
#include "InputManager.h"
#include "Time.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

Engine::Engine() = default;

Engine::~Engine()
{
    if (m_isRunning) clean();
}

bool Engine::initialize(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!m_window) return false;

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) return false;

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) return false;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_spriteShader = ResourceManager::Instance().LoadShader(
        "texture_shader",
        "shaders/vertex-tex.glsl",
        "shaders/frag-tex.glsl"
    );

    if (!m_spriteShader) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture_shader");
        return false;
    }
    
    m_playerTexture = ResourceManager::Instance().LoadTexture("Player");
    if (!m_playerTexture)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture player");

    m_enemyTexture = ResourceManager::Instance().LoadTexture("Skeleton");
    if (!m_enemyTexture)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture enemy");

    m_pickupTexture = ResourceManager::Instance().LoadTexture("Heart");
    if (!m_pickupTexture)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture heart");

    m_playButtonTexture = ResourceManager::Instance().LoadTexture("PlayButton");
    if (!m_playButtonTexture)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load PlayButton texture");

    m_youDiedTexture = ResourceManager::Instance().LoadTexture("YouDiedUI");
    if (!m_youDiedTexture)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load YouDiedUI texture");

    m_returnButtonTexture = ResourceManager::Instance().LoadTexture("ReturnMainMenuButton");
    if (!m_returnButtonTexture)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load ReturnMainMenuButton texture");

    SceneManager::Instance().LoadScene("assets/scenes/mainmenu.scene"); 
    Scene* scene = SceneManager::Instance().GetActiveScene();

    WorldGenerator worldGenerator;
    worldGenerator.Generate(*scene);

    scene->GetCamera().SetResolution(glm::vec2(width, height));

    m_spriteShader->Use();
    m_spriteShader->SetInt("texture1", 0);

    m_isRunning = true;
    return true;
}

void Engine::run()
{
    SDL_Event event;

    int windowW = 0, windowH = 0;
    SDL_GetWindowSize(m_window, &windowW, &windowH);
    glViewport(0, 0, windowW, windowH);


    while (m_isRunning)
    {
        SceneManager::Instance().ApplyPendingScene();

        Scene* scene = SceneManager::Instance().GetActiveScene();
        if (!scene) continue;

        Time::Instance().Tick();
        float dt = Time::Instance().GetDeltaTime();

        InputManager::Instance().Update();

        while (SDL_PollEvent(&event))
        {
            InputManager::Instance().ProcessEvent(event);

            if (event.type == SDL_EVENT_QUIT ||
                (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE))
            {
                m_isRunning = false;
            }

            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                int w = event.window.data1;
                int h = event.window.data2;
                scene->GetCamera().SetResolution(glm::vec2(w, h));
                glViewport(0, 0, w, h);
            }
        }

        GameObject* player = scene->FindObjectByName("Player");
        if (player)
        {
            glm::vec3 playerPos = player->GetTransform().Position;
            Camera& cam = scene->GetCamera();
            cam.GetTransform()->Position.x = playerPos.x;
            cam.GetTransform()->Position.y = playerPos.y;
            cam.GetTransform()->Position.z = -1.0f;
        }

        glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->Update(dt);
        scene->Draw();

        SDL_GL_SwapWindow(m_window);
    }
    SDL_Delay(16);
}

void Engine::clean()
{
    if (m_glContext) SDL_GL_DestroyContext(m_glContext);
    if (m_window) SDL_DestroyWindow(m_window);
    SDL_Quit();
    m_isRunning = false;
}