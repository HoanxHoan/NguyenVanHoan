#pragma once

#include "Vertex.h"
#include "Shaders.h"
#include <GLES3/gl3.h>
#include "Textures.h"
#include "Model.h"
class Object
{
public:
    Object();
    ~Object();
    //Texture
    Texture* objTex;
    //Model
    Model* objModel;

    // Matrices
    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projMatrix;
    Matrix mvpMatrix;
    // Loaders
    bool LoadModel(const char* filename);
    bool LoadTexture(const char* filename);

    bool Init(const char* modelFile, const char* textureFile);
    void SetMVP();
    void Draw(GLuint shaderProgram);
    void getViewMatrix(float out[4][4], Vector3 eye, Vector3 at, Vector3 up);
};
#pragma once
