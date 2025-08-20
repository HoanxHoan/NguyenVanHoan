#include "../stdafx.h" 
#include "GSMenu.h"
#include <iostream>

GSMenu::GSMenu() {
    Init();
}

GSMenu::~GSMenu() {
    button = nullptr;
    button2 = nullptr;
	button3 = nullptr;
    if (textRenderer) {
        delete textRenderer;
        textRenderer = nullptr;
    }
    
}

void GSMenu::HandleInput(unsigned char key, bool isPressed)
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

void GSMenu::HandleMouseClick(GLint x, GLint y, bool isClick)
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

bool GSMenu::Init()
{
    SoundManager::GetInstance()->endSound("Background");
    SoundManager::GetInstance()->endSound("slime");
    SoundManager::GetInstance()->endSound("golem");
    SoundManager::GetInstance()->endSound("death");
    SoundManager::GetInstance()->PlaySound("Background");
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Texture* tex = ResourceManager::GetInstance()->GetTexture(116);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(0);
    obj = std::make_shared<Object>(model, tex, shader);
	obj->set2Dposition(480, 320);
	obj->setSize(120, 120);
    SoundManager::GetInstance()->SetVolume(SoundManager::GetInstance()->currentVolume);
    textRenderer = new TextRenderer();
    textRenderer->Init("../Resources/Fonts/ari-w9500-bold.ttf", 20);

    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(122);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);

    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(480, 380);
    button->SetPosition(480, 430);
    button->SetSize(150, 75);
    button->setSize(40, 20);
    button->SetOnClick([]() {
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSTutorial>()); 
        });
    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(125);
    
    button2 = std::make_shared<GameButton>(btnModel, btnTexture2, btnShader);
    button2->set2Dposition(480, 400);
    button2->SetPosition(480, 500);
    button2->SetSize(150, 75);
    button2->setSize(40, 20);
    button2->SetOnClick([]() {
        printf("Option clicked");
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSOption>());
        });
    Texture* btnTexture3 = ResourceManager::GetInstance()->GetTexture(123);

    button3 = std::make_shared<GameButton>(btnModel, btnTexture3, btnShader);
    button3->set2Dposition(480, 420);
    button3->SetPosition(480, 572);
    button3->SetSize(150, 75);
    button3->setSize(40, 20);
    button3->SetOnClick([]() {
        exit(0);
        });
    return true;
}

void GSMenu::Exit()
{
    SoundManager::GetInstance()->endSound("background");
    std::cout << "Menu State Exit\n";
}

void GSMenu::Pause()
{
    std::cout << "Menu State Pause\n";
}

void GSMenu::Resume()
{
    //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);
    std::cout << "Menu State Resume\n";
}

void GSMenu::Update(float deltaTime)
{
    Camera::GetInstance()->Follow(480,360);
}

void GSMenu::Draw()
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
    if (textRenderer)
    {
        textRenderer->RenderText("© Thai & Hoan", 25.0f, 25.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
    }

}
