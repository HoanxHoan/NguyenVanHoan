#pragma once
#include "GameStateBase.h"
#include "Scene.h"
#include "GameStateMachine.h"
#include "GSPause.h"
#include "../GameObject/GameButton.h"

class GSPause : public GameStateBase
{
public:
    GSPause();
    ~GSPause();

    bool Init() override;
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void Update(float deltaTime) override;
    void Draw() override;
    void HandleInput(unsigned char key, bool isPressed) override;
    void HandleMouseClick(GLint x, GLint y, bool isClick) override;
    StateType GetStateType() const override { return StateType::PAUSE; }
    std::shared_ptr<GameButton> button_pause;
};
