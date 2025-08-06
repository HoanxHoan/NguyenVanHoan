#pragma once

#include "../Vertex.h"
#include "../Shaders.h"
#include <GLES3/gl3.h>
#include "Textures.h"
#include "Model.h"
#include "Camera.h"
#define DEG2RAD 0.0174532925199432957f
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
    virtual void Update(float deltaTime) {}
    virtual void Draw();
    float x, y;
    float width, height;
    bool CheckCollision(Object* other);
    bool CheckCollisionTree(Object* other);
    bool CheckCollisionEnermy(Object* other);
    bool CheckCollisionStone(Object* other);
    bool CheckCollisionenvir(Object* other);
    bool visible = true;
    void SetVisible(bool v) { visible = v; }
    bool IsVisible() const { return visible; }
};

