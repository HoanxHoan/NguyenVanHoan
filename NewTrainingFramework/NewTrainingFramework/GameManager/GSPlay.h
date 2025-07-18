#pragma once
#include "GameStateBase.h"
#include "Scene.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include "GSPause.h"

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

};

