#pragma once
#include "Object.h"

class Hmp : public Object
{
public:
    Hmp(Model* model, Texture* texture, Shaders* shade);
    ~Hmp();
    void SetPosition(float x, float y);
    void SetSize(float width, float height);
    void Update(float x, float y, int hp);
    void UpdateMp(float x, float y, int hp);
    float X, Y;
    float width, height;

};

