#include "SceneManager.h"
#include "Scene.h"
#include "SDL3/SDL_log.h"
#include <filesystem>

void SceneManager::LoadScene(const std::filesystem::path& filePath)
{
    std::string sceneName = filePath.stem().string();

    if (activeScene && activeScene->GetName() == sceneName)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
            "SceneManager::LoadScene request denied [%s]. Scene is already loaded.", sceneName.c_str());
        return;
    }

    if (activeScene)
    {
        activeScene->Clear();
        activeScene = nullptr;
    }

    activeScene = std::make_unique<Scene>(filePath);
    activeScene->Start();

    SDL_Log("SceneManager::LoadScene successful [%s]. Scene loaded.", sceneName.c_str());
}

void SceneManager::UnloadActiveScene()
{
    if (activeScene)
    {
        std::string sceneName = activeScene->GetName();
        activeScene->Clear();
        activeScene = nullptr;
        SDL_Log("SceneManager::UnloadScene successful [%s]. Scene unloaded.", sceneName.c_str());
    }
}

void SceneManager::LoadSceneDeferred(const std::filesystem::path& filePath)
{
    pendingScene = std::make_unique<Scene>(filePath);
}

void SceneManager::ApplyPendingScene()
{
    if (!pendingScene)
        return;

    if (activeScene)
        activeScene->Clear();

    activeScene = std::move(pendingScene);
    activeScene->Start();

    SDL_Log("SceneManager::ApplyPendingScene successful [%s]. Scene loaded.", activeScene->GetName().c_str());
}