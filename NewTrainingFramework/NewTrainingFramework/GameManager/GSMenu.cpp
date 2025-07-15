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
        if (key == 'P')
        {
            GameStateMachine::GetInstance()->ChangeState(new GSPlay());
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
        Scene::GetInstance()->Render(5);
    }
}
