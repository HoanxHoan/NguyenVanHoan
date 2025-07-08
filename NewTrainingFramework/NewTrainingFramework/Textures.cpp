#include "stdafx.h" 
#include "Textures.h"



Texture::Texture()
{
    textureID = 0;
    width = 0;
    height = 0;
    bpp = 0;
}

Texture::~Texture()
{
    if (textureID != 0)
        glDeleteTextures(1, &textureID);
}

bool Texture::LoadFromFile(const char* filePath)
{
    char* imageData = LoadTGA(filePath, &width, &height, &bpp);
    if (!imageData)
        return false;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] imageData;
    return true;
}

void Texture::Bind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
