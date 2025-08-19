#include "../stdafx.h" 
#include "GSTutorial.h"
#include <iostream>

bool popable = false;

GSTutorial::GSTutorial() {
    Init();
}

GSTutorial::~GSTutorial() {
    button = nullptr;
}

void GSTutorial::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        if (key == 27)
        {
            Exit();
        }
        if (key == 'V')
        {
            SoundManager::GetInstance()->IncreaseVolume(0.1f);
        }
        if (key == 'C')
        {
            SoundManager::GetInstance()->DecreaseVolume(0.1f);
        }
    }
}

void GSTutorial::HandleMouseClick(GLint x, GLint y, bool isClick)
{
    if (button)
    {
        button->HandleTouchEvents(x, y, isClick);

    }
}

bool GSTutorial::Init()
{
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Texture* tex = ResourceManager::GetInstance()->GetTexture(136);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(0);
    obj = std::make_shared<Object>(model, tex, shader);
    obj->set2Dposition(480, 370);
    obj->setSize(200, 170);

	Texture* tex2 = ResourceManager::GetInstance()->GetTexture(137);

    obj2 = std::make_shared<Object>(model, tex2, shader);
    obj2->set2Dposition(480, 300);
    obj2->setSize(170, 144);

    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(135);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);

    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(570, 445);
    button->SetPosition(910, 665);
    button->SetSize(50, 50);
    button->setSize(15, 15);
    button->SetOnClick([]() {
        popable = true;
        });
   
    return true;
}

void GSTutorial::Exit()
{
    popable = false;
    std::cout << "Recipe State Exit\n";
}

void GSTutorial::Pause()
{
    std::cout << "Recipe State Pause\n";
}

void GSTutorial::Resume()
{
    //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);
    std::cout << "Recipe State Resume\n";
}

void GSTutorial::Update(float deltaTime)
{
    if (popable) {
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSPlay>()); 
    }
}

void GSTutorial::Draw()
{
    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(1);
    }
    if (obj) {
        obj->Draw();
    }
    if(obj2) {
        obj2->Draw();
	}
    if (button)
    {
        button->Draw();
    }

}
