#include "stdafx.h"
#include "Scene.h"
#include "Math.h"
Scene::Scene()
{
    obj = new Object();
}

Scene::~Scene()
{
}
bool Scene::Init(ESContext* esContext)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    obj->LoadModel("../Resources/Models/Woman2.nfg");
    obj->LoadTexture("../Resources/Textures/Woman2.tga");
    return obj->initShader("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
}

void Scene::Update(ESContext* esContext, float deltaTime)
{
   
}

void Scene::Draw(ESContext* esContext)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //MVP
    obj->SetMVP();
    // Draw object
    obj->Draw();

    // Swap buffers
    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}
