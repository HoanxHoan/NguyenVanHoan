#pragma once
#include "SpriteAnimation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
class Player : public SpriteAnimation {
public:
    Player(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame,
        GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime) override;
    void MoveUp(float value, bool col);
    void MoveRight(float value, bool col);
    void MoveDown(float value, bool col);
    void MoveLeft(float value, bool col);
    void Idle(int count);
    void Crush(int action, int count);
    void Slice(int action, int count);
    void Pierce(int action, int count);
    void Dead();
    void onHit(int count,float x,float y);
    Object* GetHitbox(int count);
    ///void SetMapBoundary(float maxX, float maxY);
    bool IsWaterTile(int xPixel, int yPixel);
    //float clamp(float value, float min, float max);
    int hp, mp;
    int m_maxX, m_maxY;
    bool onhit;
    float hitdltime;
    bool isBeingKnockedBack = false;
    float knockbackTime = 0.0f;
    float knockbackDuration = 0.2f;
    float knockbackSpeed = 100.0f;
    float knockbackDirX = 0.0f;
    float knockbackDirY = 0.0f;
    float mptime;
    std::shared_ptr<Object>hitbox;
    std::vector<std::shared_ptr<Object>>* others = nullptr;
    void getObjectList(std::vector<std::shared_ptr<Object>>* Object);
    std::vector<int> waterTiles;
    void getwaterTiles(std::vector<int>& waterTiles);
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    int mapPixelWidth;
    int mapPixelHeight;
};

