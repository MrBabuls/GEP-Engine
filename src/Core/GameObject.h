#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Component.h"
#include "Transform.h"

class Camera;
class Scene;

class GameObject {
public:
	GameObject(const std::string& name) : name(name), parentScene(nullptr) {}

	auto Start() -> void;
	auto Update(float deltaTime) -> void;
	auto Draw(const Camera& camera) const -> void;

	inline auto& GetName() const { return name; }
	inline auto& GetTransform() { return transform; }

	inline Scene* GetScene() { return parentScene; }
	inline void SetScene(Scene* scene) { parentScene = scene; }

	template<typename T, typename ...TArgs>
	auto AddComponent(TArgs&& ...args) -> T*;

	template<typename T>
	auto GetComponent() const -> T*;

private:
	std::string name;
	Transform transform;

	std::vector<std::unique_ptr<IComponent>> components;
	std::vector<IDrawableComponent*> drawableComponents;

	Scene* parentScene;
};

template<typename T, typename ...TArgs>
auto GameObject::AddComponent(TArgs&& ...args) -> T*
{
	static_assert(std::is_base_of_v<IComponent, T>, "T must be a component type inheriting from IComponent");

	auto component = std::make_unique<T>(this, std::forward<TArgs>(args)...);
	T* ptr = component.get();

	if (parentScene)
	{
		ptr->SetScene(parentScene);
	}

	components.push_back(std::move(component));

	if (auto drawable = dynamic_cast<IDrawableComponent*>(ptr))
	{
		drawableComponents.push_back(drawable);
	}

	return ptr;
}

template<typename T>
auto GameObject::GetComponent() const -> T*
{
	for (const auto& component : components)
	{
		if (T* result = dynamic_cast<T*>(component.get()))
		{
			return result;
		}
	}

	return nullptr;
}