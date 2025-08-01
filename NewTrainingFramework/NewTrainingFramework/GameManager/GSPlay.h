#pragma once

#include "GameStateBase.h"
#include "Scene.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include "GSPause.h"
#include "../GameObject/SpriteAnimation.h"
#include "../GameObject/Building.h"
#include "../GameObject/Environment.h"
#include "../GameObject/Enemy.h"
#include "../GameObject/Overlay.h"
#include "../tinyxml2.h"
class GSPlay : public GameStateBase
{
public:
    GSPlay();
    ~GSPlay();

    bool Init() override;
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void Update(float deltaTime) override;
    void Draw() override;
    void HandleInput(unsigned char key, bool isPressed) override;
    void HandleMouseClick(GLint x, GLint y, bool isClick) override;
    StateType GetStateType() const override { return StateType::PLAY; }
    bool IsWaterTile(int xPixel, int yPixel);
    tinyxml2::XMLDocument doc;
    std::shared_ptr<GameButton> button_play;
    std::shared_ptr<GameButton> button_play2;
    std::shared_ptr<SpriteAnimation> P1;
    std::shared_ptr<Building>bonfire;
    std::shared_ptr<Building>fire;
    std::shared_ptr<Environment>tree;
    std::shared_ptr<Enemy>org;
    std::shared_ptr<Overlay> overlay;
    std::vector<Vector2> lights;
    bool keyState[256] = { false };
    int x, y, count,action;
    float dltime = 0.0f, actiontime = 0.0f;
    std::vector<Object*> i_objects;
    std::vector<Object*> i_bonfire;
    std::vector<int> waterTiles;
    int mapWidth ;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    int mapPixelWidth ;
    int mapPixelHeight ;
};

