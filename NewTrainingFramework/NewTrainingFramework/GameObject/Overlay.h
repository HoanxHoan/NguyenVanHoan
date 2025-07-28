#pragma once
#include "Object.h"

class OverlayObject : public Object {
public:
    float alpha = 0.5f; 

    OverlayObject(Model* model, Texture* texture, Shaders* shader)
        : Object(model, texture, shader) {
    }

    void Draw();
};

