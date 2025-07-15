#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>

GSPlay::GSPlay() {

}
GSPlay::~GSPlay() {

}
void GSPlay::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        if (key == 'R')
        {
            GameStateMachine::GetInstance()->ChangeState(new GSMenu());
        }
        else if (key == 27)
        {
            Exit();
        }
    }
}
bool GSPlay::Init()
{
    return 0;
}

void GSPlay::Exit()
{
    std::cout << "Play State Exit\n";
}

void GSPlay::Pause()
{
    std::cout << "Play State Pause\n";
}

void GSPlay::Resume()
{
    std::cout << "Play State Resume\n";
}

void GSPlay::Update(float deltaTime)
{
    // Update gameplay logic here
}

void GSPlay::Draw()
{
    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(2);
        Scene::GetInstance()->Render(4);
        Scene::GetInstance()->Render(3);
        Scene::GetInstance()->Render(6);
    }
}
