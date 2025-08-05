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

    void Update(GLfloat deltaTime, std::vector<std::shared_ptr<Object>>& others) ;
    void Draw() override;

    std::vector<int> waterTiles;
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    int mapPixelWidth;
    int mapPixelHeight;
private:
    enum Action {
        Idle,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    Action currentAction;
    float actionTime;     
    float maxActionTime;  
    float speed;
    bool IsWaterTile(int xPixel, int yPixel);
    void ChooseNewAction();
    void PerformAction(GLfloat deltaTime,std::vector<std::shared_ptr<Object>>& others);
};
