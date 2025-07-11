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
    Object(Model* m,Texture* t,Shaders* s, Matrix modelM, Matrix translationM,Matrix rotationM,Matrix scaleM);
    ~Object();
    //Texture
    Texture* objTex;
    //Model
    Model* objModel;
    //Shader
    Shaders* objShader;
    // Matrices
    Matrix translationMatrix ;
    Matrix rotationMatrix;
    Matrix scaleMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    Matrix projMatrix;
    Matrix mvpMatrix;
    // Loaders
    void updateRotation(Matrix m);
    void SetMVP();
    void Draw();

};
#pragma once
