#include "../stdafx.h" 
#include "GSPause.h"
#include <iostream>

GSPause::GSPause() {
}

GSPause::~GSPause() {
}

void GSPause::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        if (key == 'R')
        {
            GameStateMachine::GetInstance()->PopState();
        }
        else if (key == 27)
        {
            Exit();
        }
    }
}

bool GSPause::Init()
{
    return 0;
}

void GSPause::Exit()
{
    std::cout << "Menu State Exit\n";
}

void GSPause::Pause()
{
    std::cout << "Menu State Pause\n";
}

void GSPause::Resume()
{
    std::cout << "Menu State Resume\n";
}

void GSPause::Update(float deltaTime)
{

}

void GSPause::Draw()
{
    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(2);
        Scene::GetInstance()->Render(7);
    }
}
