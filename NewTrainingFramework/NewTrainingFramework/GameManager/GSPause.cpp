#include "../stdafx.h" 
#include "GSPause.h"
#include <iostream>
std::shared_ptr<GameButton> button_pause;
GSPause::GSPause() {
    Init();
}

GSPause::~GSPause() {
    button_pause = nullptr;
}

void GSPause::HandleInput(unsigned char key, bool isPressed)
{
        if (key == 27)
        {
            Exit();
        }
    
}
void GSPause::HandleMouseClick(GLint x, GLint y, bool isClick)
{
    if (button_pause)
    {
        button_pause->HandleTouchEvents(x, y, isClick);
    }

}
bool GSPause::Init()
{
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(3);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    button_pause = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button_pause->set2Dposition(480, 360);
    button_pause->SetPosition(480, 360);
    button_pause->setSize(200, 200);
    button_pause->SetSize(200, 200);
    button_pause->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });

    return true;
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
    }
    if (button_pause)
    {
        button_pause->Draw();
    }
}
