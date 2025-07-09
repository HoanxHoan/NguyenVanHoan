#include <stdafx.h>
#include "../stdafx.h"
#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Initialize static instance pointer
ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    // Free all models
    for (auto& m : models)
        delete m.second;

    // Free all textures
    for (auto& t : textures)
        delete t.second;

    // Free all shaders
    for (auto& s : shaders)
        delete s.second;
}

ResourceManager* ResourceManager::GetInstance()
{
    if (instance == nullptr)
        instance = new ResourceManager();
    return instance;
}

void ResourceManager::Destroy()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

bool ResourceManager::LoadFileRM(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Cannot open ResourceManager file.\n";
        return false;
    }

    std::string line;
    while (getline(file, line))
    {
        if (line.find("#Model") != std::string::npos)
        {
            int count;
            std::istringstream ss(line);
            std::string tag;
            ss >> tag >> count;
            for (int i = 0; i < count; ++i)
            {
                int id;
                std::string filePath;
                file >> line >> id;
                file >> line >> filePath;

                Model* model = new Model();
                model->LoadNFG(filePath.c_str());
                models[id] = model;
                std::cout << "Loaded Model ID " << id << ": " << filePath << std::endl;
            }
        }
        else if (line.find("#2DTexture") != std::string::npos)
        {
            int count;
            std::istringstream ss(line);
            std::string tag;
            ss >> tag >> count;
            for (int i = 0; i < count; ++i)
            {
                int id;
                std::string filePath, tiling;
                file >> line >> id;
                file >> line >> filePath;
                file >> line >> tiling;
                GLint wrapMode = GL_REPEAT; // default

                if (tiling == "GL_REPEAT")
                    wrapMode = GL_REPEAT;
                else if (tiling == "GL_CLAMP_TO_EDGE")
                    wrapMode = GL_CLAMP_TO_EDGE;
                else if (tiling == "GL_MIRRORED_REPEAT")
                    wrapMode = GL_MIRRORED_REPEAT;
                Texture* tex = new Texture();
                tex->LoadFromFile(filePath.c_str(), wrapMode, wrapMode);
                textures[id] = tex;
                std::cout << "Loaded Texture ID " << id << ": " << filePath << std::endl;
            }
        }
        else if (line.find("#Shader") != std::string::npos)
        {
            int count;
            std::istringstream ss(line);
            std::string tag;
            ss >> tag >> count;
            for (int i = 0; i < count; ++i)
            {
                int id;
                std::string vs, fs;
                file >> line >> id;
                file >> line >> vs;
                file >> line >> fs;

                Shaders* shader = new Shaders();
                if (shader->Init((char*)vs.c_str(), (char*)fs.c_str())!=0)
                {
                    std::cout << "Shader load failed for ID " << id << std::endl;
                    delete shader;
                    continue;
                }
                shaders[id] = shader;
            }
        }
    }

    file.close();
    return true;
}


Model* ResourceManager::GetModel(int ID)
{
    if (models.find(ID) != models.end()) {
        std::cout << 1;
        return models[ID];
    }
    else
        return nullptr;
}

Texture* ResourceManager::GetTexture(int ID)
{
    if (textures.find(ID) != textures.end()) {
        std::cout << 2;
        return textures[ID];
    }
    else
        return nullptr;
}

Shaders* ResourceManager::GetShader(int ID)
{
    if (shaders.find(ID) != shaders.end())
    {
        shaders[ID]->Init(shaders[ID]->fileVS, shaders[ID]->fileFS);
        std::cout << 3;
        return shaders[ID];
    }
    else
        return nullptr;
}
