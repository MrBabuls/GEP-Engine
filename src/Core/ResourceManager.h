#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Texture;
class Shader;

class ResourceManager
{
public:
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    std::shared_ptr<Texture> LoadTexture(const std::string& name = "");
    std::shared_ptr<Shader>  LoadShader(const std::string& name, const std::string& vert, const std::string& frag);

    std::shared_ptr<Texture> GetTexture(const std::string& name);
    std::shared_ptr<Shader> GetShader(const std::string& name);

private:
    ResourceManager() = default;

    std::unordered_map<std::string, std::weak_ptr<Texture>> m_TextureCache;
    std::unordered_map<std::string, std::weak_ptr<Shader>>  m_ShaderCache;
};