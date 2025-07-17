#include "../stdafx.h" 
#include "GSMenu.h"
#include <iostream>
std::shared_ptr<GameButton> button;
GSMenu::GSMenu() {
    Init();
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
void GSMenu::HandleMouseClick(GLint x, GLint y, bool isClick)
{
    if (button)
    {
        button->HandleTouchEvents(x, y, isClick);
    }
}

bool GSMenu::Init()
{
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(6);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(480,360);
    button->SetPosition(480, 360);
    button->setSize(200, 200);
    button->SetSize(200, 200);
    button->SetOnClick([]() {
        GameStateMachine::GetInstance()->PushState(new GSPlay());
        });
    return true;
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
    }
    if (button)
    {
        button->Draw();
    }
}
