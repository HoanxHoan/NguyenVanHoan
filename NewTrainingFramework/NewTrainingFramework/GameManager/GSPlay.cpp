#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
std::shared_ptr<GameButton> button_play;
GSPlay::GSPlay() {
    Init();
}
GSPlay::~GSPlay() {

}
void GSPlay::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        if (key == 'R')
        {
            GameStateMachine::GetInstance()->PopState();
        }else if (key == 'P')
        {
            GameStateMachine::GetInstance()->PushState(new GSPause());
        }
        else if (key == 27)
        {
            Exit();
        }
    }
}
void GSPlay::HandleMouseClick(GLint x, GLint y, bool isClick)
{
    if (button_play)
    {
        button_play->HandleTouchEvents(x, y, isClick);
    }
}
bool GSPlay::Init()
{
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(3);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    button_play = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button_play->set2Dposition(850, 70);
    button_play->SetPosition(850, 70);
    button_play->setSize(100, 100);
    button_play->SetSize(100, 100);
    button_play->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    return true;
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
    }
    if (button_play)
    {
        button_play->Draw();
    }
}
