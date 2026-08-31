#include "ResourceManager.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"
#include <iostream>

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& name)
{
    if (auto cached = m_TextureCache[name].lock())
        return cached;

    auto tex = std::make_shared<Texture>();
    
    std::string extensions[] = { ".png", ".jpg", ".jpeg" };
    std::string folders[] = { "assets/textures/", "assets/UI/" };
    bool loaded = false;

    for (auto& folder : folders)
    {
        for (auto& ext : extensions)
        {
            std::string path = folder + name + ext;
            if (tex->LoadFromFile(path))
            {
                loaded = true;
                SDL_Log("[Load] Texture loaded: %s", path.c_str());
                break;
            }
        }
        if (loaded) break;
    }

    if (!loaded)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", name.c_str());
        return nullptr;
    }

    m_TextureCache[name] = tex;
    return tex;
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name, const std::string& vert, const std::string& frag)
{
    auto it = m_ShaderCache.find(name);
    if (it != m_ShaderCache.end())
        if (auto cached = it->second.lock())
            return cached;

    auto shader = std::make_shared<Shader>(vert, frag);

    if (shader->GetId() == 0)
    {
        std::cerr << "[ResourceManager] Shader compilation failed for: " << name << std::endl;
        return nullptr;
    }

    m_ShaderCache[name] = shader;
    std::cout << "[Load] Shader loaded: " << name << std::endl;
    return shader;
}


std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
{
    if (auto cached = m_TextureCache[name].lock())
        return cached;
    return nullptr;
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    if (auto cached = m_ShaderCache[name].lock())
        return cached;
    return nullptr;
}