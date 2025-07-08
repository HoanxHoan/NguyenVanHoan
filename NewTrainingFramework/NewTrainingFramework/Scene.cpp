#include "stdafx.h"
#include "Scene.h"
#include "Math.h"
#define DEG2RAD 0.0174532925199432957f
Scene::Scene()
{
}

Scene::~Scene()
{
}
void Scene::getViewMatrix(float out[4][4], Vector3 eye, Vector3 at, Vector3 up)
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
bool Scene::Init(ESContext* esContext)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    obj.LoadModel("../Resources/Models/Woman1.nfg");
    obj.LoadTexture("../Resources/Textures/Woman1.tga");

    return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Scene::Update(ESContext* esContext, float deltaTime)
{
   
}

void Scene::Draw(ESContext* esContext)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Model matrix
    Matrix modelMatrix;
    modelMatrix.SetIdentity();

    Matrix scaleMatrix;
    scaleMatrix.SetScale(0.5f, 0.5f, 0.5f);

    Matrix rotationMatrix;
    rotationMatrix.SetRotationY(70.0f * DEG2RAD);

    Matrix translationMatrix;
    translationMatrix.SetTranslation(0.0f, 1.0f, 7.0f);

    modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

    // View matrix
    Matrix viewMatrix;
    Vector3 eye(0.0f, 1.0f, 3.0f);
    Vector3 at(0.0f, 1.0f, 0.0f);
    Vector3 up(0.0f, 1.0f, 0.0f);
    getViewMatrix(viewMatrix.m, eye, at, up);

    // Projection matrix
    Matrix projMatrix;
    projMatrix.SetPerspective(45.0f * DEG2RAD, 4.0f / 3.0f, 0.1f, 100.0f);

    // Set MVP
    obj.SetMVP(modelMatrix, viewMatrix, projMatrix);

    // Draw object
    obj.Draw(myShaders.program);

    // Swap buffers
    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}
