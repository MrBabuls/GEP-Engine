#include "Texture.h"
#include <iostream>

Texture::Texture() {}

Texture::Texture(const std::string& filePath)
{
    LoadFromFile(filePath);
}

Texture::~Texture()
{
    if (id)
        glDeleteTextures(1, &id);
}

bool Texture::LoadFromFile(const std::string& filePath)
{
    this->filePath = filePath;

    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) 
    {
        return false;
    }

    width = surface->w;
    height = surface->h;

    // Flip rows vertically (OpenGL expects (0,0) bottom-left)
    unsigned char* pixels = static_cast<unsigned char*>(surface->pixels);
    int pitch = surface->pitch;
    unsigned char* flipped = new unsigned char[pitch * height];
    for (int y = 0; y < height; ++y)
        memcpy(flipped + y * pitch, pixels + (height - 1 - y) * pitch, pitch);

    // Determine texture format (RGB vs RGBA)
    GLenum format = GL_RGB;
   
    const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(surface->format);
    if (details && details->Amask)
        format = GL_RGBA;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, flipped);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL texture error: " << std::hex << error << std::dec << std::endl;
        delete[] flipped;
        SDL_DestroySurface(surface);
        return false;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] flipped;
    SDL_DestroySurface(surface);

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}