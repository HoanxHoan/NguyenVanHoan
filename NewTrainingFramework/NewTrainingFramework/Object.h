#pragma once

#include "Vertex.h"
#include "Shaders.h"
#include <GLES3/gl3.h>

class Object
{
public:
    Object();
    ~Object();
    // Buffer IDs
    GLuint vboId;
    GLuint iboId;
    GLuint textureID;

    // Counts
    int vertexCount;
    int indexCount;

    // Matrices
    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projMatrix;

    // Loaders
    bool LoadModel(const char* filename);
    bool LoadTexture(const char* filename);

    bool Init(const char* modelFile, const char* textureFile);
    void SetMVP(Matrix model, Matrix view, Matrix proj);
    void Draw(GLuint shaderProgram);

};
#pragma once
