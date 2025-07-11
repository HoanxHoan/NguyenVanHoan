
#include "../stdafx.h"
#include "Object.h"
#include <cstdio>
#include <iostream>
#define DEG2RAD 0.0174532925199432957f
Object::Object(Model* m, Texture* t, Shaders* s, Matrix modelM)
{
    objModel = m;
    objTex = t;
    objShader = s;
    modelMatrix = modelM;
    //translationMatrix = translation;
    //rotationMatrix = rotation;
    //scaleMatrix = scale;
}

Object::~Object()
{
    objTex = nullptr;
    objModel = nullptr;
    objShader = nullptr;

}

void Object::SetMVP()
{
    //Camera::GetInstance()->UpdateViewMatrix();
    // View matrix
    viewMatrix = Camera::GetInstance()->viewMatrix;

    // Projection matrix
    projMatrix = Camera::GetInstance()->projMatrix;
    //MVP
    mvpMatrix = modelMatrix * viewMatrix * projMatrix;
}


void Object::Draw()
{
    glUseProgram(objShader->program);
    glEnable(GL_DEPTH_TEST);
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