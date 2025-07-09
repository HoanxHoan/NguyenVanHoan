#include "stdafx.h" 
#include "Textures.h"


Texture::Texture()
{
    textureID = 0;
}

Texture::~Texture()
{
    if (textureID != 0)
        glDeleteTextures(1, &textureID);
}

bool Texture::LoadFromFile(const char* filePath, GLint wrapS, GLint wrapT)
{
    int width, height, bpp;
    char* imageData = LoadTGA(filePath, &width, &height, &bpp);

    if (!imageData)
    {
        std::cout << "Failed to load texture.\n";
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] imageData;
    std::cout << "Loaded texture: " << filePath <<"\n";
    return true;
}
void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
