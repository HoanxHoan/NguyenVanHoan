#pragma once
#include "GameStateBase.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "GameStateMachine.h"
#include "GSPlay.h"
class GSMenu : public GameStateBase
{
public:
    GSMenu();
    ~GSMenu();

    bool Init() override;
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void Update(float deltaTime) override;
    void Draw() override;
    void HandleInput(unsigned char key, bool isPressed) override;
    StateType GetStateType() const override { return StateType::MENU; }

};
