#include "../stdafx.h" 
#include "GSPause.h"
#include <iostream>

bool shouldPop = false;

GSPause::GSPause() {
    Init();
}

GSPause::~GSPause() {
    button = nullptr;
    button2 = nullptr;
    button3 = nullptr;
}

void GSPause::HandleInput(unsigned char key, bool isPressed)
{
    if (isPressed)
    {
        if (key == 27)
        {
            Exit();
        }
    }
}

void GSPause::HandleMouseClick(GLint x, GLint y, bool isClick)
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

bool GSPause::Init()
{

    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Texture* tex = ResourceManager::GetInstance()->GetTexture(117);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(0);
    obj = std::make_shared<Object>(model, tex, shader);
    obj->set2Dposition(480, 360);
    obj->setSize(70, 100);

    //textRenderer = new TextRenderer();
    //textRenderer->Init("../Resources/Fonts/arial.ttf", 48);

    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(121);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);

    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(480, 350);
    button->SetPosition(480, 320);
    button->SetSize(150, 75);
    button->setSize(40, 20);
    button->SetOnClick([]() {
        shouldPop = true;
        });
    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(125);

    button2 = std::make_shared<GameButton>(btnModel, btnTexture2, btnShader);
    button2->set2Dposition(480, 370);
    button2->SetPosition(480, 395);
    button2->SetSize(150, 75);
    button2->setSize(40, 20);
    button2->SetOnClick([]() {
        //printf("Option clicked");
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSOption>());
        });
    Texture* btnTexture3 = ResourceManager::GetInstance()->GetTexture(124);

    button3 = std::make_shared<GameButton>(btnModel, btnTexture3, btnShader);
    button3->set2Dposition(480, 390);
    button3->SetPosition(480, 465);
    button3->SetSize(150, 75);
    button3->setSize(40, 20);
    button3->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(std::make_unique<GSMenu>());
        });
    return true;
}

void GSPause::Exit()
{
    shouldPop = false;
    std::cout << "Menu State Exit\n";
}

void GSPause::Pause()
{
    std::cout << "Menu State Pause\n";
}

void GSPause::Resume()
{
    //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);
    std::cout << "Menu State Resume\n";
}

void GSPause::Update(float deltaTime)
{
    Camera::GetInstance()->Follow(480, 360);

    //if (!clickable)
    //{
    //    dltime += deltaTime;
    //}

    //if (dltime >= 0.5f)
    //{
    //    clickable = true;
    //    dltime = 0.0f;
    //}

    //if (muted)
    //{
    //    button->objTex = ResourceManager::GetInstance()->GetTexture(118);
    //    SoundManager::GetInstance()->SetVolume(0.0f);
    //}
    //else
    //{
    //    button->objTex = ResourceManager::GetInstance()->GetTexture(119);
    //    // Lấy trực tiếp từ currentVolume thay vì biến volume trong GSOption
    //    SoundManager::GetInstance()->SetVolume(SoundManager::GetInstance()->currentVolume);
    //}
    if (shouldPop) {
        GameStateMachine::GetInstance()->PopState();
    }
}

void GSPause::Draw()
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
    //if (textRenderer)
    //{
    //    int volumePercent = static_cast<int>(round(SoundManager::GetInstance()->currentVolume * 100.0f));

    //    if (volumePercent == 100) {
    //        textRenderer->RenderText(std::to_string(volumePercent), 440.0f, 310.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
    //    }
    //    else if (volumePercent < 10) {
    //        textRenderer->RenderText(std::to_string(volumePercent), 470.0f, 310.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
    //    }
    //    else {
    //        textRenderer->RenderText(std::to_string(volumePercent), 455.0f, 310.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
    //    }
    //}

}
