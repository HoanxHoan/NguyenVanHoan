#pragma once

#include <map>
#include <string>
#include "../GameObject/Model.h"
#include "../GameObject/Textures.h"
#include "../Shaders.h"

class ResourceManager
{
private:
    // Singleton instance
    static ResourceManager* instance;

    // Private constructor
    ResourceManager();

public:
    // Destructor
    ~ResourceManager();

    // Get singleton instance
    static ResourceManager* GetInstance();

    // Destroy singleton instance
    static void Destroy();

    // Resource maps
    std::map<int, Model*> models;
    std::map<int, Texture*> textures;
    std::map<int, Shaders*> shaders;

    // Functions
    bool LoadFileRM(const char* filename);

    Model* GetModel(int ID);
    Texture* GetTexture(int ID);
    Shaders* GetShader(int ID);
};
