#pragma once
#include "Scene.h" 
#include <string>
#include <memory>
#include <filesystem>

class SceneManager
{
public:
    static auto& Instance()
    {
        static SceneManager instance;
        return instance;
    }

    void LoadScene(const std::filesystem::path& filePath);
    void UnloadActiveScene();

    void LoadSceneDeferred(const std::filesystem::path& filePath);
    void ApplyPendingScene();

    inline Scene* GetActiveScene() const { return activeScene.get(); }

public:
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

private:
    SceneManager() = default;

    std::unique_ptr<Scene> activeScene;
    std::unique_ptr<Scene> pendingScene;
};