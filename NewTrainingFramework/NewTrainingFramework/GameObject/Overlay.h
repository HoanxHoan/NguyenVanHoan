#pragma once
#include "Object.h"
#include "../Utilities/utilities.h"
#include "../Vertex.h"
#include "../Shaders.h"
#include <vector>
class Overlay : public Object {
public:
    float alpha; 
    int x, y;
    Overlay(Model* model, Texture* texture, Shaders* shader)
        : O_Model(model), O_Texture(texture), O_Shader(shader), alpha(0.0f) {
    }
    void SetLights(const std::vector<Vector2>& lights) {
        lightPositions = lights;
    }
    void SetLightRadius(float r) { radius = r; }
    void SetLightSoftness(float s) { softness = s; }
    void Draw();
    void SetOverlayPosition(GLfloat px, GLfloat py);
    void SetOverlaySize(GLfloat sx,GLfloat sy);
    void GetgameTime(float t);
private:
    Vector3 scale;
    Model* O_Model;
    Shaders* O_Shader;
    Texture* O_Texture;
    std::vector<Vector2> lightPositions;
    float gameTime = 0.5;
    float radius = 150.0f;
    float softness = 100.0f;

};

