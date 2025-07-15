#include "../stdafx.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "ResourceManager.h"
#define DEG2RAD 0.0174532925199432957f

Scene::Scene()
{
}

Scene::~Scene()
{
    Cleanup();
}

bool Scene::Init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    ResourceManager::GetInstance()->LoadFileRM("GameManager/ResourceManager.txt");
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
        }
        else if (line.find("ID") != std::string::npos)
        {
            int id, modelID, textureID, shaderID;
            float px, py, pz, rx, ry, rz, sx, sy, sz;

            std::istringstream ss(line);
            std::string temp;
            ss >> temp >> id;

            fin >> temp >> modelID;
            fin >> temp >> textureID;
            fin >> temp >> shaderID;
            fin >> temp >> px >> py >> pz;
            fin >> temp >> rx >> ry >> rz;
            fin >> temp >> sx >> sy >> sz;

            Model* objModel = ResourceManager::GetInstance()->GetModel(modelID);
            Texture* objTex = ResourceManager::GetInstance()->GetTexture(textureID);
            Shaders* objShader = ResourceManager::GetInstance()->GetShader(shaderID);

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

            Object* obj = new Object(objModel, objTex, objShader, modelMatrix, translationMatrix, rotationMatrix, scaleMatrix);
            m_objects.push_back(obj);
        }
        else if (line.find("#CAMERA") != std::string::npos)
        {
            float nearPlane, farPlane, fov, speed;
            fin >> line >> nearPlane;
            fin >> line >> farPlane;
            fin >> line >> fov;
            fin >> line >> speed;

            Camera::GetInstance()->SetNearFar(nearPlane, farPlane);
            Camera::GetInstance()->SetFOV(fov);
            Camera::GetInstance()->SetSpeed(speed);
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
        // Update logic here if needed
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
