#include "Scene.h"
#include <fstream>
#include <cstdint>
#include <glm/fwd.hpp>
#include "nlohmann/json.hpp"
#include "ComponentRegistry.h"


Scene::Scene() : camera(Camera::DefaultCamera())
{

}

Scene::Scene(const std::filesystem::path& filePath)
    : camera(Camera::DefaultCamera())
{
    LoadFromFile(filePath);
}

Scene::~Scene()
{
    Clear();
}

void Scene::Start()
{
	camera.GetTransform()->Position = { 0.0f, 0.0f, -1.0f };

    for (const auto& gameObjects : gameObjects)
    {
        gameObjects->Start();
    }
}

void Scene::Update(float deltaTime)
{
    for (const auto& gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}

void Scene::Draw() const
{
    for (const auto& gameObject : gameObjects)
    {
        gameObject->Draw(GetCamera());
    }
}

void Scene::Clear()
{
    gameObjects.clear();
}

void Scene::OnWindowResize(uint32_t width, uint32_t height)
{
	camera.SetResolution(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
}

GameObject* Scene::FindObjectByName(const std::string& name)
{
	for (auto& obj : gameObjects)
	{
		if (obj->GetName() == name)
		{
			return obj.get();
		}
	}
	return nullptr;
}

void Scene::LoadFromFile(const std::filesystem::path& filePath)
{
	name = filePath.stem().string();
	std::ifstream file{ filePath };
	auto data = nlohmann::json::parse(file);
	Deserialize(data);
}

void Scene::Deserialize(const nlohmann::json& data)
{
	if (data.contains("camera"))
	{
		auto& position = data["camera"]["position"];
		camera.GetTransform()->Position = glm::vec3(
			position[0].get<float>(),
			position[1].get<float>(),
			position[2].get<float>()
		);
	}

	if (!data.contains("game_objects"))
	{
		return;
	}

	for (const auto& gameObjectData : data["game_objects"])
	{
		auto name = gameObjectData["name"].get<std::string>();
		gameObjects.emplace_back(std::make_unique<GameObject>(name));
		auto* gameObject = gameObjects.back().get();

		gameObject->SetScene(this);

		if (gameObjectData.contains("position"))
		{
			auto& position = gameObjectData["position"];
			gameObject->GetTransform().Position = glm::vec3(
				position[0].get<float>(),
				position[1].get<float>(),
				position[2].get<float>());
		}

		if (gameObjectData.contains("components"))
		{
			for (const auto& componentData : gameObjectData["components"])
			{
				std::string type = componentData["type"].get<std::string>();
				auto* component = ComponentRegistry::Instance().Create(type, gameObject);
				if (component)
				{
					component->Deserialize(componentData);
				}
				else
				{
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Component type not registered: %s", type.c_str());
				}
			}
		}
	}
}