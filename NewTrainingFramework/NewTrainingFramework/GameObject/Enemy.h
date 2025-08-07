#pragma once
#include "SpriteAnimation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
class Enemy : public SpriteAnimation {
public:
    Enemy(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;
    void moveTo(float x,float y, float deltaTime);
    void Dead();
    void onHit(int count);
    void endHit();
    int hp, type;
    std::vector<std::shared_ptr<Object>>* others = nullptr;
    void getObjectList(std::vector<std::shared_ptr<Object>>* Object);
private:
    float dltime, deaddltime;
    bool death, isHit;
};

