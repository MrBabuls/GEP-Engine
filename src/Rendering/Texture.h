#pragma once
#include <string>
#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Texture {
public:
    Texture();
    Texture(const std::string& filePath);
    ~Texture();

    bool LoadFromFile(const std::string& filePath);

    unsigned int GetId() const { return id; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    unsigned int id = 0;
    int width = 0;
    int height = 0;
    std::string filePath;
};