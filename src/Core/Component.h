#pragma once
#include <nlohmann/json_fwd.hpp>

class GameObject;
class Camera;
class Scene;

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void Deserialize(const nlohmann::json& serializedData) = 0;

	void SetScene(Scene* scene) { parentScene = scene; }
	Scene* GetScene() { return parentScene; }

protected:
	IComponent(GameObject* owner) : owner(owner), parentScene(nullptr) {}
	GameObject* owner;
	Scene* parentScene;
};

class IDrawableComponent : public IComponent
{
public:
	virtual void Draw(const Camera& camera) const = 0;

protected:
	IDrawableComponent(GameObject* owner) : IComponent(owner) {}
};