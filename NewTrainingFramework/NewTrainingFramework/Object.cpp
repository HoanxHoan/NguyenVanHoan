#include "stdafx.h"
#include "Object.h"
#include <cstdio>
#include <iostream>
#define DEG2RAD 0.0174532925199432957f
Object::Object()
{
    objTex = new Texture();
    objModel = new Model();
    objShader = new Shaders();
    modelMatrix.SetIdentity();
    viewMatrix.SetIdentity();
    projMatrix.SetIdentity();
}

Object::~Object()
{
    if (objTex) delete objTex;
    if (objModel) delete objModel;
}
void Object::getViewMatrix(float out[4][4], Vector3 eye, Vector3 at, Vector3 up)
{
    Vector3 zaxis = (at - eye).Normalize();
    Vector3 xaxis = up.Cross(zaxis).Normalize();
    Vector3 yaxis = zaxis.Cross(xaxis);

    out[0][0] = xaxis.x;
    out[0][1] = yaxis.x;
    out[0][2] = zaxis.x;
    out[0][3] = 0;

    out[1][0] = xaxis.y;
    out[1][1] = yaxis.y;
    out[1][2] = zaxis.y;
    out[1][3] = 0;

    out[2][0] = xaxis.z;
    out[2][1] = yaxis.z;
    out[2][2] = zaxis.z;
    out[2][3] = 0;

    out[3][0] = -xaxis.Dot(eye);
    out[3][1] = -yaxis.Dot(eye);
    out[3][2] = -zaxis.Dot(eye);
    out[3][3] = 1;
}
bool Object::Init(const char* modelFile, const char* textureFile)
{
    if (!LoadModel(modelFile))
        return false;
    if (!LoadTexture(textureFile))
        return false;

    return true;
}
bool Object::initShader(char* filename1,char* filename2) {
    return objShader->Init(filename1,filename2);
}
bool Object::LoadModel(const char* filename)
{
    objModel->LoadNFG(filename);
    return true;
}

bool Object::LoadTexture(const char* filename)
{
    objTex->LoadFromFile(filename);
    return true;
}

void Object::SetMVP()
{
    // Model matrix
    modelMatrix.SetIdentity();

    Matrix scaleMatrix;
    scaleMatrix.SetScale(0.5f, 0.5f, 0.5f);

    Matrix rotationMatrix;
    rotationMatrix.SetRotationY(70.0f * DEG2RAD);

    Matrix translationMatrix;
    translationMatrix.SetTranslation(0.0f, 1.0f, 7.0f);

    modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    // View matrix
    Vector3 eye(0.0f, 1.0f, 3.0f);
    Vector3 at(0.0f, 1.0f, 0.0f);
    Vector3 up(0.0f, 1.0f, 0.0f);
    getViewMatrix(viewMatrix.m, eye, at, up);

    // Projection matrix
    projMatrix.SetPerspective(45.0f * DEG2RAD, 4.0f / 3.0f, 0.1f, 100.0f);
    //MVP
    mvpMatrix = modelMatrix * viewMatrix * projMatrix;
}


void Object::Draw()
{
    glUseProgram(objShader->program);

    // Bind Texture
    objTex->Bind();
    int iTextureLoc = glGetUniformLocation(objShader->program, "u_texture");
    glUniform1i(iTextureLoc, 0);

    // Set MVP
    GLuint mvpLoc = glGetUniformLocation(objShader->program, "u_mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvpMatrix.m);
    objModel->SetAttributes();

    // Draw
    glDrawElements(GL_TRIANGLES, objModel->indexCount, GL_UNSIGNED_INT, 0);
    objTex->Unbind();
    objModel->unBind();

}
