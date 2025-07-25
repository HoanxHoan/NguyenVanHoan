#include "../stdafx.h" 
#include "GSMenu.h"
#include <iostream>

GSMenu::GSMenu() {
    Init();
}

GSMenu::~GSMenu() {
    button = nullptr;
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
}

bool GSMenu::Init()
{
    SoundManager::GetInstance()->Init();
    SoundManager::GetInstance()->LoadSound("bgm", "../Resources/Sound/sound_1.wav");
    SoundManager::GetInstance()->PlaySound("bgm");
    SoundManager::GetInstance()->SetVolume(SoundManager::GetInstance()->currentVolume);
    textRenderer = new TextRenderer();
    textRenderer->Init("../Resources/Fonts/arial.ttf", 48);

    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(6);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);

    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(480, 360);
    button->SetPosition(480, 360);
    button->setSize(200, 200);
    button->SetSize(200, 200);

    button->SetOnClick([]() {
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSPlay>()); 
        });

    return true;
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
    }
    if (button)
    {
        button->Draw();
    }
    if (textRenderer)
    {
        textRenderer->RenderText("Hoan", 25.0f, 25.0f, 1.0f, Vector3(0.3f, 0.7f, 0.9f));
    }
}
