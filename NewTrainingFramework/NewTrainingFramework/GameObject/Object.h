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
    Object();
    Object(Model* m,Texture* t,Shaders* s);
    Object(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Shaders> shader);
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
    void set2Dposition(float x,float y);
    void setSize(GLint w, GLint h);
    void SetMVP();
    //void SetTranslationMatrix();
    //void SetRotationMatrix();
    //void SetScaleMatrix();
    void Draw();
   
};

