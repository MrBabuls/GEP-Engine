#pragma once
#include <string>
#include <unordered_map>
#include "SDL3/SDL_log.h"

class IComponent;
class GameObject;

class ComponentRegistry
{
public:
	using ComponentCreator = auto (*)(GameObject*)->IComponent*;

	static auto& Instance()
	{
		static ComponentRegistry instance;
		return instance;
	}

	auto Register(const std::string& typeName, ComponentCreator creatorFunction) -> void;
	auto Create(const std::string& typeName, GameObject* owner) -> IComponent*;

	template<typename T>
	struct Registerer
	{
		Registerer(const std::string& name);
	};

public:

	ComponentRegistry(const ComponentRegistry&) = delete;
	ComponentRegistry& operator=(const ComponentRegistry&) = delete;
	ComponentRegistry(ComponentRegistry&&) = delete;
	ComponentRegistry& operator=(ComponentRegistry&&) = delete;

private:
	ComponentRegistry() = default;

	std::unordered_map<std::string, ComponentCreator> creatorFunctions{};
};

inline auto ComponentRegistry::Register(const std::string& typeName, ComponentCreator creatorFunction) -> void
{
	auto [it, inserted] = creatorFunctions.try_emplace(typeName, creatorFunction);
	if (!inserted)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ComponentRegistry::Register request denied [%s]. Creator function already exists.", typeName.c_str());
	}
}

inline auto ComponentRegistry::Create(const std::string& typeName, GameObject* owner) -> IComponent*
{
	auto it = creatorFunctions.find(typeName);
	if (it == creatorFunctions.end())
	{
		return nullptr;
	}

	return it->second(owner);
}

template<typename T>
inline ComponentRegistry::Registerer<T>::Registerer(const std::string& name)
{
	ComponentRegistry::Instance().Register(name, [](GameObject* owner) -> IComponent*
		{
			return owner->AddComponent<T>();
		});
}

#define REGISTER_COMPONENT(TYPE) static ComponentRegistry::Registerer<TYPE> reg_##TYPE(#TYPE);