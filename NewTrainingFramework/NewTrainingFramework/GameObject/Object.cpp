
#include "../stdafx.h"
#include "Object.h"
#include <cstdio>
#include <iostream>
#define DEG2RAD 0.0174532925199432957f
Object::Object(){
    objModel = nullptr;
    objTex = nullptr;
    objShader = nullptr;
    modelMatrix.SetIdentity();
    translationMatrix.SetIdentity();
    rotationMatrix.SetIdentity();
    scaleMatrix.SetIdentity();
}
Object::Object(std::shared_ptr<Model> model, std::shared_ptr<Texture> texture, std::shared_ptr<Shaders> shader) {
    objModel = model.get();
    objTex = texture.get();
    objShader = shader.get();

    modelMatrix.SetIdentity();
    translationMatrix.SetIdentity();
    rotationMatrix.SetIdentity();
    scaleMatrix.SetIdentity();
}
Object::Object(Model* m, Texture* t, Shaders* s)
{
    objModel = m;
    objTex = t;
    objShader = s;
    modelMatrix.SetIdentity();
    translationMatrix.SetIdentity();
    rotationMatrix.SetIdentity();
    scaleMatrix.SetIdentity();
}

Object::~Object()
{
    objTex = nullptr;
    objModel = nullptr;
    objShader = nullptr;

}
void Object::setSize(GLint w, GLint h) {
    width = w*2/3;
    height = h*2/3;
    scaleMatrix.SetScale((GLfloat)w, (GLfloat)h, 1.0f);

}
void Object::set2Dposition(float x, float y) {
    this->x = x;
    this->y = y;
    translationMatrix.SetTranslation(x, y, 0.0f);
    
}
void Object::updateRotation(Matrix m) {
    rotationMatrix = rotationMatrix * m;
}
bool Object::CheckCollision(Object* other)
{
    return !(x + width / 2 < other->x - other->width / 2 ||
        x - width / 2 > other->x + other->width / 2 ||
        y + height / 2 < other->y - other->height / 2 ||
        y - height / 2 > other->y + other->height / 2);
}
void Object::SetMVP()
{

    //Camera::GetInstance()->UpdateViewMatrix();
    modelMatrix = scaleMatrix * rotationMatrix * translationMatrix ;
    // View matrix
    viewMatrix = Camera::GetInstance()->viewMatrix;

    // Projection matrix
    projMatrix = Camera::GetInstance()->projMatrix;
    //MVP
    mvpMatrix = modelMatrix * viewMatrix * projMatrix;
}
void Object::Draw()
{
    if (!visible) return;
    glUseProgram(objShader->program);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    objTex->Bind();
    int iTextureLoc = glGetUniformLocation(objShader->program, "u_texture");
    glUniform1i(iTextureLoc, 0);
    SetMVP();
    GLuint mvpLoc = glGetUniformLocation(objShader->program, "u_mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvpMatrix.m);

    objModel->SetAttributes();

    glDrawElements(GL_TRIANGLES, objModel->indexCount, GL_UNSIGNED_INT, 0);

    objTex->Unbind();
    objModel->unBind();
}