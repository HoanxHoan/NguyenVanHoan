#include "../stdafx.h" 
#include "GSOption.h"
#include <iostream>

float clickable = true;


bool muted = false;
GSOption::GSOption() {
    Init();
}

GSOption::~GSOption() {
    button = nullptr;
    button2 = nullptr;
    button3 = nullptr;
	button4 = nullptr;
    if (textRenderer) {
        delete textRenderer;
        textRenderer = nullptr;
    }

    
}

void GSOption::HandleInput(unsigned char key, bool isPressed)
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

void GSOption::HandleMouseClick(GLint x, GLint y, bool isClick)
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
    if (button4)
    {
        button4->HandleTouchEvents(x, y, isClick);

    }
}

bool GSOption::Init()
{

    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Texture* tex = ResourceManager::GetInstance()->GetTexture(117);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(0);
    obj = std::make_shared<Object>(model, tex, shader);
    obj->set2Dposition(480, 360);
    obj->setSize(70, 100);

    textRenderer = new TextRenderer();
    textRenderer->Init("../Resources/Fonts/ari-w9500-bold.ttf", 48);

    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(119);
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    button = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button->set2Dposition(480, 340);
    button->SetPosition(480, 280);
    button->SetSize(150, 150);
    button->setSize(35, 35);
    button->SetOnClick([]() {
        if(clickable)
        {
            muted = !muted;
            
            clickable = false;
        }
        });

    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(127);
    button2 = std::make_shared<GameButton>(btnModel, btnTexture2, btnShader);
    button2->set2Dposition(460, 370);
    button2->SetPosition(380, 395);
    button2->SetSize(50, 50);
    button2->setSize(15, 15);
    button2->SetOnClick([]() {
        if (clickable)
        {
            printf("Increased clicked\n");
            SoundManager::GetInstance()->IncreaseVolume(0.1f);
        }
        });
    Texture* btnTexture3 = ResourceManager::GetInstance()->GetTexture(128);

    button3 = std::make_shared<GameButton>(btnModel, btnTexture3, btnShader);
    button3->set2Dposition(500, 370);
    button3->SetPosition(580, 395);
    button3->SetSize(50, 50);
    button3->setSize(15, 15);
    button3->SetOnClick([]() {
        if (clickable)
        {
            printf("Decreased clicked\n");
            SoundManager::GetInstance()->DecreaseVolume(0.1f);
        }
        });

    Texture* btnTexture4 = ResourceManager::GetInstance()->GetTexture(126);

    button4 = std::make_shared<GameButton>(btnModel, btnTexture4, btnShader);
    button4->set2Dposition(480, 390);
    button4->SetPosition(480, 470);
    button4->SetSize(150, 75);
    button4->setSize(40, 20);
    button4->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    return true;
}

void GSOption::Exit()
{
    std::cout << "Menu State Exit\n";
}

void GSOption::Pause()
{
    std::cout << "Menu State Pause\n";
}

void GSOption::Resume()
{
    //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);
    std::cout << "Menu State Resume\n";
}

void GSOption::Update(float deltaTime)
{
    Camera::GetInstance()->Follow(480, 360);

    if (!clickable)
    {
        dltime += deltaTime;
    }

    if (dltime >= 0.5f)
    {
        clickable = true;
        dltime = 0.0f;
    }

    if (muted)
    {
        button->objTex = ResourceManager::GetInstance()->GetTexture(118);
        SoundManager::GetInstance()->SetVolume(0.0f);
    }
    else
    {
        button->objTex = ResourceManager::GetInstance()->GetTexture(119);
        // Lấy trực tiếp từ currentVolume thay vì biến volume trong GSOption
        SoundManager::GetInstance()->SetVolume(SoundManager::GetInstance()->currentVolume);
    }
}

void GSOption::Draw()
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
		button4->Draw();
    }
    if (textRenderer)
    {
        int volumePercent = static_cast<int>(round(SoundManager::GetInstance()->currentVolume * 100.0f));

        if (volumePercent == 100) {
            textRenderer->RenderText(std::to_string(volumePercent), 440.0f, 310.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
        }
        else if (volumePercent < 10) {
            textRenderer->RenderText(std::to_string(volumePercent), 470.0f, 310.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
        }
        else {
            textRenderer->RenderText(std::to_string(volumePercent), 455.0f, 310.0f, 1.0f, Vector3(1.0f, 1.0f, 1.0f));
        }
    }

}
