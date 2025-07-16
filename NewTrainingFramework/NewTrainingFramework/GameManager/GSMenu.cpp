#include "../stdafx.h" 
#include "GSMenu.h"
#include <iostream>

GSMenu::GSMenu() {
}

GSMenu::~GSMenu() {
}

void GSMenu::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        if (key == 'S')
        {
            GameStateMachine::GetInstance()->PushState(new GSPlay());
        }
        else if (key == 'P')
        {
            GameStateMachine::GetInstance()->PushState(new GSPause());
        }
        else if (key == 27) 
        {
            Exit();
        }
    }
}

bool GSMenu::Init()
{
    return 0;
}

void GSMenu::Exit()
{
    exit(0);
    std::cout << "Menu State Exit\n";
}

void GSMenu::Pause()
{
    std::cout << "Menu State Pause\n";
}

void GSMenu::Resume()
{
    std::cout << "Menu State Resume\n";
}

void GSMenu::Update(float deltaTime)
{
  
}

void GSMenu::Draw()
{
    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(2);
        Scene::GetInstance()->Render(4);
        Scene::GetInstance()->Render(3);
        Scene::GetInstance()->Render(6);
    }
}
