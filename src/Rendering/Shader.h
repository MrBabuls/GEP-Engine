#pragma once
#include <string>
#include <glad/gl.h> 
#include <glm/glm.hpp>

class Shader {
public:
    // Constructor: compile and link shaders from files
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);

    unsigned int GetId() const;

    // Uniform setters
    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    void SetBool(const std::string& name, bool value) const;

    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;

    void Use() const { glUseProgram(id); }

private:
    unsigned int id = 0;

    std::string LoadShaderCode(const std::string& path) const;
    void CheckCompileErrors(unsigned int shader, const std::string& type) const;
};