#pragma once
#include "GameStateBase.h"
#include <stack>
#include <memory>

class GameStateMachine
{
public:
    ~GameStateMachine();

    static GameStateMachine* GetInstance();
    static void Destroy();

    void ChangeState(std::unique_ptr<GameStateBase> state);
    void PushState(std::unique_ptr<GameStateBase> state);
    void PopState();
    GameStateBase* CurrentState();

    void Update(float deltaTime);
    void Draw();
    GameStateMachine();
    static std::unique_ptr<GameStateMachine> instance;
    std::stack<std::unique_ptr<GameStateBase>> m_StateStack;
};
