#pragma once
#include "SpriteAnimation.h"
#include "../GameManager/PlayerInventory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
class Boss : public SpriteAnimation {
public:
    Boss(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void Draw() override;
    void moveTo(float x, float y, float deltaTime);
    void Dead();
    void onHit(int count, float damage);
    void endHit();
    int hp, type,atk;
    float dame;
    bool spawn, death;
    //std::vector<std::shared_ptr<Object>>* others = nullptr;
    //void getObjectList(std::vector<std::shared_ptr<Object>>* Object);
private:
    float dltime, deaddltime;
    bool isHit;
    bool isBeingKnockedBack = false;
    float knockbackTime = 0.0f;
    float knockbackDuration = 0.1f;
    float knockbackSpeed = 20.0f;
    float knockbackDirX = 0.0f;
    float knockbackDirY = 0.0f;
};
