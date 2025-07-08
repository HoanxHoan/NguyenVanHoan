#pragma once

#include <GLES3/gl3.h>
#include <iostream>
#include "Vertex.h"
#include "Shaders.h"
class Model
{
public:
    Model();
    ~Model();

    bool LoadNFG(const char* filename);
    void SetAttributes();
    void unBind();
    GLuint vboId;
    GLuint iboId;
    int indexCount;

    Vertex* verticesData;
    unsigned int* indices;
};
