#pragma once
#include "SpriteAnimation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
class Animal : public SpriteAnimation {
public:
    Animal(Model* model, Shaders* shader, Texture* texture,
        GLint numFrames, GLint currentFrame, GLint numActions, GLint currentAction, GLfloat frameTime);

    void Update(GLfloat deltaTime)override;
    void Draw() override;
    void getObjectList(std::vector<std::shared_ptr<Object>>* others);
    void getwaterTiles(std::vector<int>& waterTiles);
    void onHit();
    void endHit();
    void dead();
    int hp, type;
    std::vector<int> waterTiles;
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    int mapPixelWidth;
    int mapPixelHeight;
    std::vector<std::shared_ptr<Object>>* others = nullptr;;
private:
    enum Action {
        Idle,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    Action currentAction;
    float actionTime,dltime,hpdltime,deaddltime;     
    float maxActionTime;  
    float speed;
    bool isHit,isdead;
    bool IsWaterTile(int xPixel, int yPixel);
    void ChooseNewAction();
    void PerformAction(GLfloat deltaTime);
};
