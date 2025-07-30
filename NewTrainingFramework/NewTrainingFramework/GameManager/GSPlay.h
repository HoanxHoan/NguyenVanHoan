#pragma once

#include "GameStateBase.h"
#include "Scene.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include "GSPause.h"
#include "../GameObject/SpriteAnimation.h"
#include "../GameObject/Overlay.h"
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

    std::shared_ptr<GameButton> button_play;
    std::shared_ptr<GameButton> button_play2;
    std::shared_ptr<SpriteAnimation> spriteAnim;
    std::shared_ptr<Overlay> overlay;
    std::shared_ptr<Overlay> overlay2;
    std::shared_ptr<Overlay> overlay3;
    std::vector<Vector2> lights;
    bool keyState[256] = { false };
    int x, y, count;
    float dltime = 0.0f, pdltime = 0.0f;
    Object* P1;
    Object* P2;
    std::vector<Object*> i_objects;
};

