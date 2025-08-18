#pragma once
#include "GameStateBase.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include "../GameObject/GameButton.h"
#include "../GameObject/TextRenderer.h"
#include <memory>

class GSOption : public GameStateBase
{
public:
    GSOption();
    ~GSOption();

    bool Init() override;
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void Update(float deltaTime) override;
    void Draw() override;
    void HandleInput(unsigned char key, bool isPressed) override;
    void HandleMouseClick(GLint x, GLint y, bool isClick) override;
    StateType GetStateType() const override { return StateType::MENU; }
private:
    TextRenderer* textRenderer;
    std::shared_ptr<GameButton> button;
    std::shared_ptr<GameButton> button2;
    std::shared_ptr<GameButton> button3;
    std::shared_ptr<GameButton> button4;
    std::shared_ptr<Object> obj;
};
