#pragma once
#include <GLES3/gl3.h>
#include <cstdio>
#include <iostream>
char* LoadTGA(const char* filePath, int* width, int* height, int* bpp);

class Texture
{
public:

    unsigned int textureID;

    // Constructor
    Texture();

    // Destructor
    ~Texture();

    // Load texture 
    bool LoadFromFile(const char* filePath);

    // Bind texture 
    void Bind();

    // Unbind texture
    void Unbind();
};
