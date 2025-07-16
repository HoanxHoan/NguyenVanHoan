#include "../stdafx.h"
#include "GSIntro.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include <iostream>

GSIntro::GSIntro()
{
    introTime = 3.0f; 
    elapsedTime = 0.0f;
}

GSIntro::~GSIntro() {}

bool GSIntro::Init()
{
    std::cout << "Intro Init\n";
    return true;
}

void GSIntro::Exit()
{
    std::cout << "Intro Exit\n";
}

void GSIntro::Pause() {}
void GSIntro::Resume() {}

void GSIntro::Update(float deltaTime)
{
    elapsedTime += deltaTime;
    if (elapsedTime >= introTime)
    {
        
        GameStateMachine::GetInstance()->ChangeState(new GSMenu());
    }
}

void GSIntro::Draw()
{
    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(8);
    }
    Update(0.05);
    std::cout << "Drawing Intro Screen\n";
}

void GSIntro::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        GameStateMachine::GetInstance()->ChangeState(new GSMenu());
    }
}
