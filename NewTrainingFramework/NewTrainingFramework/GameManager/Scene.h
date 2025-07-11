#pragma once
#include "../Shaders.h"
#include "../GameObject/Object.h"
#include "../GameObject/Textures.h"
#include "../GameObject/Camera.h"
#include <GLES3/gl3.h>
#include "ResourceManager.h"
#include <vector>


class Scene
{
private:
    static Scene* instance;
    std::vector<Object*> m_objects;

    Scene(); // private constructor for singleton

public:
    ~Scene();

    // Singleton getInstance
    static Scene* GetInstance();
    static void Destroy();

    // Core functions
    bool Init(); // initialize scene manager
    bool LoadFileSM(const char* file); // load scene file

    void Update(float deltaTime); // update objects
    void Render(int id); // draw all objects

    void Cleanup(); // clear all objects

    // Getter
    const std::vector<Object*>& GetObjects() const { return m_objects; }
};

