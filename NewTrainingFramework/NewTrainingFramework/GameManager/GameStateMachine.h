#pragma once
#include "GameStateBase.h"
#include <stack>

class GameStateMachine
{
private:
    static GameStateMachine* instance;
    std::stack<GameStateBase*> m_StateStack;

    GameStateMachine(); // private constructor

public:
    ~GameStateMachine();

    static GameStateMachine* GetInstance();
    static void Destroy();

    void ChangeState(GameStateBase* state);
    void PushState(GameStateBase* state);
    void PopState();
    GameStateBase* CurrentState();

    void Update(float deltaTime);
    void Draw();
};
