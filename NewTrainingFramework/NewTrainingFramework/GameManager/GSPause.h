#pragma once
#include "GameStateBase.h"
#include "Scene.h"
#include "GameStateMachine.h"
#include "GSPause.h"
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
    StateType GetStateType() const override { return StateType::PAUSE; }


};
