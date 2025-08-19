#pragma once
#include "SpriteAnimation.h"
#include "../GameManager/PlayerInventory.h"
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
    void DeadnoItem();
    void onHit(int count, float damage);
    void endHit();
    int hp, type;
    float dame;
    bool spawn;
    std::vector<std::shared_ptr<Object>>* others = nullptr;
    void getObjectList(std::vector<std::shared_ptr<Object>>* Object);
    bool isdead;
private:
    float dltime, deaddltime;
    bool death, isHit;
    bool isBeingKnockedBack = false;
    float knockbackTime = 0.0f;
    float knockbackDuration = 0.1f;
    float knockbackSpeed = 50.0f; 
    float knockbackDirX = 0.0f;
    float knockbackDirY = 0.0f;
};

