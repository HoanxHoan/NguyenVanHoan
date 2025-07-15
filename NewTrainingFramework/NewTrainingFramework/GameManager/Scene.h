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
    std::vector<Object*> m_objects;

public:
    Scene();
    ~Scene();

    bool Init(); // initialize scene manager
    bool LoadFileSM(const char* file); // load scene file

    void Update(float deltaTime); // update objects
    void Render(int id); // draw object by id
    void Cleanup(); // clear all objects

    const std::vector<Object*>& GetObjects() const { return m_objects; }
};
