#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <memory>

#include <nlohmann/json_fwd.hpp>

#include "../Rendering/Camera.h"
#include "GameObject.h"

class Texture;

class Scene
{
public:
    Scene();
    Scene(const std::filesystem::path& filePath);

    ~Scene();
    Scene(const Scene&) = delete;
    Scene operator=(const Scene&) = delete;

    inline auto& GetName() const { return name; }
    inline auto& GetCamera() const { return camera; }
    inline auto& GetCamera() { return camera; }
    inline const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return gameObjects; }

    void Start();
    void Update(float deltaTime);
    void Draw() const;
    void Clear();
    void OnWindowResize(uint32_t width, uint32_t height);

    void SetPlayerAlive(bool alive) { m_playerAlive = alive; }
    bool IsPlayerAlive() const { return m_playerAlive; }

    GameObject* FindObjectByName(const std::string& name);

private:
    Camera camera;
    std::string name;
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    void LoadFromFile(const std::filesystem::path& filePath);
    void Deserialize(const nlohmann::json& data);

    bool m_playerAlive = true;
};