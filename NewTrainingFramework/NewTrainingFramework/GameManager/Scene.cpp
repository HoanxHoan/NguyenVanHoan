#include "../stdafx.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "ResourceManager.h"
#define DEG2RAD 0.0174532925199432957f
Scene* Scene::instance = nullptr;

Scene::Scene()
{
}

Scene::~Scene()
{
    Cleanup();
}

Scene* Scene::GetInstance()
{
    if (!instance)
        instance = new Scene();
    return instance;
}

void Scene::Destroy()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

bool Scene::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Load ResourceManager first if needed
    ResourceManager::GetInstance()->LoadFileRM("GameManager/ResourceManager.txt");
    // Load scene file
    return LoadFileSM("GameManager/SceneManager.txt");
}

bool Scene::LoadFileSM(const char* file)
{
    std::ifstream fin(file);
    if (!fin.is_open())
    {
        std::cout << "Failed to open scene file.\n";
        return false;
    }

    std::string line;
    int objectCount = 0;

    while (getline(fin, line))
    {
        if (line.find("#ObjectCount") != std::string::npos)
        {
            std::istringstream ss(line);
            std::string tag;
            ss >> tag >> objectCount;
            //fin >> line >> objectCount;
        }
        else if (line.find("ID") != std::string::npos)
        {

            int id, modelID, textureID, shaderID;
            float px, py, pz, rx, ry, rz, sx, sy, sz;

            // Read current ID line
            std::istringstream ss(line);
            std::string temp;
            ss >> temp >> id;

            // Read MODEL_ID
            fin >> temp >> modelID;
            // Read TEXTURE_ID
            fin >> temp >> textureID;
            // Read SHADER_ID
            fin >> temp >> shaderID;
            // Read POS
            fin >> temp >> px >> py >> pz;
            // Read ROTATION
            fin >> temp >> rx >> ry >> rz;
            // Read SCALE
            fin >> temp >> sx >> sy >> sz;

            // Get resources from ResourceManager
            Model* objModel = ResourceManager::GetInstance()->GetModel(modelID);
            Texture* objTex = ResourceManager::GetInstance()->GetTexture(textureID);
            Shaders* objShader = ResourceManager::GetInstance()->GetShader(shaderID);

            // Set transform
            Matrix translationMatrix;
            Matrix rotationMatrix;
            Matrix scaleMatrix;
            Matrix modelMatrix;
            modelMatrix.SetIdentity();
            scaleMatrix.SetScale(sx, sy, sz);
            Matrix RotationXMatrix;
            Matrix RotationYMatrix;
            Matrix RotationZMatrix;
            RotationXMatrix.SetRotationX(rx * DEG2RAD);
            RotationYMatrix.SetRotationY(ry * DEG2RAD);
            RotationZMatrix.SetRotationZ(rz * DEG2RAD);
            rotationMatrix = RotationXMatrix * RotationYMatrix * RotationZMatrix;
            translationMatrix.SetTranslation(px, py, pz);

            modelMatrix = scaleMatrix * rotationMatrix * translationMatrix;

            Object* obj = new Object(objModel, objTex, objShader, modelMatrix);

            m_objects.push_back(obj);
        }
    }

    fin.close();

    std::cout << "Loaded " << m_objects.size() << " objects from scene file.\n";
    return true;
}

void Scene::Update(float deltaTime)
{
    for (auto obj : m_objects)
    {
    }
}

void Scene::Render(int id)
{
    if (id >= 0 && id < m_objects.size())
    {
        m_objects[id]->Draw();

    }

}

void Scene::Cleanup()
{
    for (auto obj : m_objects)
    {
        delete obj;
    }
    m_objects.clear();
}
