#include "../stdafx.h"
#include "GameStateMachine.h"
#include <iostream>

GameStateMachine* GameStateMachine::instance = nullptr;

GameStateMachine::GameStateMachine()
{
}

GameStateMachine::~GameStateMachine()
{
    while (!m_StateStack.empty())
    {
        m_StateStack.top()->Exit();
        delete m_StateStack.top();
        m_StateStack.pop();
    }
}

GameStateMachine* GameStateMachine::GetInstance()
{
    if (!instance)
        instance = new GameStateMachine();
    return instance;
}

void GameStateMachine::Destroy()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void GameStateMachine::ChangeState(GameStateBase* state)
{
    if (!m_StateStack.empty())
    {
        m_StateStack.top()->Exit();
        delete m_StateStack.top();
        m_StateStack.pop();
    }
    m_StateStack.push(state);
}

void GameStateMachine::PushState(GameStateBase* state)
{
    if (!m_StateStack.empty())
        m_StateStack.top()->Pause();

    m_StateStack.push(state);

}

void GameStateMachine::PopState()
{
    if (!m_StateStack.empty())
    {
        m_StateStack.top()->Exit();
        delete m_StateStack.top();
        m_StateStack.pop();
    }

    if (!m_StateStack.empty())
        m_StateStack.top()->Resume();
}

GameStateBase* GameStateMachine::CurrentState()
{
    if (!m_StateStack.empty())
        return m_StateStack.top();
    return nullptr;
}

void GameStateMachine::Update(float deltaTime)
{
    if (!m_StateStack.empty())
        m_StateStack.top()->Update(deltaTime);
}

void GameStateMachine::Draw()
{
    if (!m_StateStack.empty())
        m_StateStack.top()->Draw();
}
