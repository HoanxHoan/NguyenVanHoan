#pragma once
#include "GameStateBase.h"
#include <memory>

class Object;

class GSIntro : public GameStateBase
{
public:
    GSIntro();
    ~GSIntro();

    bool Init() override;
    void Exit() override;
    void Pause() override;
    void Resume() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void HandleInput(unsigned char key, bool isPressed) override;
    void HandleMouseClick(GLint x, GLint y, bool isClick) override;
    StateType GetStateType() const override { return StateType::INTRO; }

private:
    float introTime;
    float elapsedTime;
    Object* obj;
};
