#include "GameObject.h"
#include "Scene.h"
#include "../Rendering/Camera.h"

auto GameObject::Start() -> void
{
	for (const auto& component : components)
	{
		component->Start();
	}
}

auto GameObject::Update(float deltaTime) -> void
{
	for (const auto& component : components)
	{
		component->Update(deltaTime);
	}
}

auto GameObject::Draw(const Camera& camera) const -> void
{
	for (const auto& drawableComponent : drawableComponents)
	{
		drawableComponent->Draw(camera);
	}
}