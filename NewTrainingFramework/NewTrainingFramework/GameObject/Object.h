#pragma once

#include "../Vertex.h"
#include "../Shaders.h"
#include <GLES3/gl3.h>
#include "Textures.h"
#include "Model.h"
#include "Camera.h"
class Object
{
public:
    Object(Model* m,Texture* t,Shaders* s, Matrix modelM);
    ~Object();
    //Texture
    Texture* objTex;
    //Model
    Model* objModel;
    //Shader
    Shaders* objShader;
    // Matrices

    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projMatrix;
    Matrix mvpMatrix;
    // Loaders
    void SetMVP();
    void Draw();

};
#pragma once
