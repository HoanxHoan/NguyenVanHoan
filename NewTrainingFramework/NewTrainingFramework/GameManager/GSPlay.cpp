#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
std::shared_ptr<GameButton> button_play;
bool keyState[256];
int x, y;
Object* P1;
GSPlay::GSPlay() {
    Init();
}
GSPlay::~GSPlay() {

}
void GSPlay::HandleInput(unsigned char key, bool isPressed)
{
        keyState[key] = isPressed;
        if (isPressed)
        {
            if (key == 27)
            {
                Exit();
            }
            if (key == 'R')
            {
                GameStateMachine::GetInstance()->PopState();
            }
            if (key == 'P')
            {
                GameStateMachine::GetInstance()->PushState(new GSPause());
            }
        }
        else {
            P1->objTex = ResourceManager::GetInstance()->GetTexture(7);
            if (y == 1) {
                x++;
                y = 0;
            }
            if (y == 2) {
                x++;
                y = 0;
            }
            if (y == 3) {
                x++;
                y = 0;
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
    Model* P1Model = ResourceManager::GetInstance()->GetModel(2);
    Texture* P1Texture = ResourceManager::GetInstance()->GetTexture(7);
    Shaders* P1Shader = ResourceManager::GetInstance()->GetShader(0);
    P1 = new Object(P1Model, P1Texture, P1Shader);
    P1->x = 300;
    P1->y = 200;
    P1->set2Dposition(P1->x, P1->y);
    P1->setSize(100, 100);
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
    if (keyState['W'] )
    {
        if (P1->y >= 20) { P1->y -= 10; }
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['D'])
    {
        if(P1->x <= 900){ P1->x += 10;}
        P1->objTex = ResourceManager::GetInstance()->GetTexture(8);
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['A'])
    {
        if (P1->x >= 20) { P1->x -= 10; }
        P1->objTex = ResourceManager::GetInstance()->GetTexture(9);
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['S'] )
    {
        if (P1->y <= 650) { P1->y += 10; }
        P1->objTex = ResourceManager::GetInstance()->GetTexture(8);
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['J'])
    {       
        if (x % 3 == 0) { 
            P1->objTex = ResourceManager::GetInstance()->GetTexture(10);
            y = 1;
        }
        if (x % 3 == 1) {
            P1->objTex = ResourceManager::GetInstance()->GetTexture(11);
            y = 2;
        }
        if (x % 3 == 2) {
            P1->objTex = ResourceManager::GetInstance()->GetTexture(12);
            y = 3;
        }
    }

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
    if (P1)
    {
        P1->Draw();
    }
}
