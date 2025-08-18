#include "../stdafx.h" 
#include "GSRecipe.h"
#include <iostream>

bool canPop = false;
int i = 0;

GSRecipe::GSRecipe() {
    Init();
}

GSRecipe::~GSRecipe() {
    button = nullptr;
    button2 = nullptr;
    button3 = nullptr;
}

void GSRecipe::HandleInput(unsigned char key, bool isPressed)
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

void GSRecipe::HandleMouseClick(GLint x, GLint y, bool isClick)
{
    if (button)
    {
        button->HandleTouchEvents(x, y, isClick);

    }
    if (button2)
    {
        button2->HandleTouchEvents(x, y, isClick);

    }
    if (button3)
    {
        button3->HandleTouchEvents(x, y, isClick);

    }
}

bool GSRecipe::Init()
{
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Texture* tex = ResourceManager::GetInstance()->GetTexture(132);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(0);
    obj = std::make_shared<Object>(model, tex, shader);
    obj->set2Dposition(480, 370);
    obj->setSize(120, 120);
    
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(130);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);

    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(430, 415);
    button->SetPosition(240, 555);
    button->SetSize(50, 50);
    button->setSize(15, 15);
    button->SetOnClick([]() {
        if (i > 0) {
            i--;
        }
        printf("%d\n", i);
        });
    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(131);

    button2 = std::make_shared<GameButton>(btnModel, btnTexture2, btnShader);
    button2->set2Dposition(535, 415);
    button2->SetPosition(745, 555);
    button2->SetSize(50, 50);
    button2->setSize(15, 15);
    button2->SetOnClick([]() {
        if(i < 2){
            i++;
        }
        printf("%d\n", i);
        });
    Texture* btnTexture3 = ResourceManager::GetInstance()->GetTexture(126);

    button3 = std::make_shared<GameButton>(btnModel, btnTexture3, btnShader);
    button3->set2Dposition(480, 430);
    button3->SetPosition(480, 610);
    button3->SetSize(150, 75);
    button3->setSize(40, 20);
    button3->SetOnClick([]() {
		canPop = true;
        });
    return true;
}

void GSRecipe::Exit()
{
	canPop = false;
    std::cout << "Recipe State Exit\n";
}

void GSRecipe::Pause()
{
    std::cout << "Recipe State Pause\n";
}

void GSRecipe::Resume()
{
    //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);
    std::cout << "Recipe State Resume\n";
}

void GSRecipe::Update(float deltaTime)
{
    Camera::GetInstance()->Follow(480, 360);
    
    switch (i) {
    case 0:
		obj->objTex = ResourceManager::GetInstance()->GetTexture(132);
		break;
    case 1:
		obj->objTex = ResourceManager::GetInstance()->GetTexture(133);
        break;
	case 2:
		obj->objTex = ResourceManager::GetInstance()->GetTexture(134);
    }
    if (canPop) {
        GameStateMachine::GetInstance()->PopState();
    }
}

void GSRecipe::Draw()
{
    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(1);
    }
    if (obj) {
        obj->Draw();
    }
    if (button)
    {
        button->Draw();
        button2->Draw();
        button3->Draw();
    }

}
