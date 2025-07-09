#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

bool Scene::Init(ESContext* esContext)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Load resource files
    ResourceManager::GetInstance()->LoadFileRM("ResourceManager.txt");
    Model* objModel = ResourceManager::GetInstance()->GetModel(1);
    Texture* objTex = ResourceManager::GetInstance()->GetTexture(1);
    Shaders* objShader = ResourceManager::GetInstance()->GetShader(0);
     obj = new Object(objModel, objTex, objShader);
    
    if (!obj->objModel || !obj->objTex || !obj->objShader)
    {
        std::cout << "Resource loading failed!\n";
        return false;
    }
    return true;
}

void Scene::Update(ESContext* esContext, float deltaTime)
{
    // Update logic if needed
}

void Scene::Draw(ESContext* esContext)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    obj->Draw();

    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}
