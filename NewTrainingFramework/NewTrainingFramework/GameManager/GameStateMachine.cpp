#include "../stdafx.h"
#include "GameStateMachine.h"
#include <iostream>

std::unique_ptr<GameStateMachine> GameStateMachine::instance = nullptr;

GameStateMachine::GameStateMachine()
{
}

GameStateMachine::~GameStateMachine()
{
    while (!m_StateStack.empty())
    {
        m_StateStack.top()->Exit();
        m_StateStack.pop(); 
    }
}

GameStateMachine* GameStateMachine::GetInstance()
{
    if (!instance)
        instance = std::make_unique<GameStateMachine>();
    return instance.get();
}

void GameStateMachine::Destroy()
{
    if (instance)
    {
        instance.reset(); 
    }
}

void GameStateMachine::ChangeState(std::unique_ptr<GameStateBase> state)
{
    if (!m_StateStack.empty())
    {
        m_StateStack.top()->Exit();
        m_StateStack.pop();
    }
    m_StateStack.push(std::move(state));
}

void GameStateMachine::PushState(std::unique_ptr<GameStateBase> state)
{
    if (!m_StateStack.empty())
        m_StateStack.top()->Pause();

    m_StateStack.push(std::move(state));
}

void GameStateMachine::PopState()
{
    if (!m_StateStack.empty())
    {
        m_StateStack.top()->Exit();
        m_StateStack.pop();
    }

    if (!m_StateStack.empty())
        m_StateStack.top()->Resume();
}

GameStateBase* GameStateMachine::CurrentState()
{
    if (!m_StateStack.empty())
        return m_StateStack.top().get();
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
