#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>

GSPlay::GSPlay() {
    x = 0; y = 0; count = 0;
    dltime = 0.0f;
    pdltime = 0.0f;
    Init();
}
GSPlay::~GSPlay() {
    if (P1) {
        delete P1;
        P1 = nullptr;
    }
}

void GSPlay::HandleInput(unsigned char key, bool isPressed)
{
        keyState[key] = isPressed;
        if (isPressed)
        {
            count = 0;
            if (key == 27)
            {
                Exit();
            }

        }
        else {
            count = 1;
            if (spriteAnim->GetCurrentFrame() == 4) {
                spriteAnim->SetFrameTime(0.7);
            }
            //P1->objTex = ResourceManager::GetInstance()->GetTexture(16); 
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
    if (button_play && button_play->HandleTouchEvents(x, y, isClick)) {
        return; 
    }
    if (button_play2 && button_play2->HandleTouchEvents(x, y, isClick)) {
        return;
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
    Model* btnModel2 = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(6);
    Shaders* btnShader2 = ResourceManager::GetInstance()->GetShader(0);
    button_play2 = std::make_shared<GameButton>(btnModel2, btnTexture2, btnShader2);
    button_play2->set2Dposition(700, 70);
    button_play2->SetPosition(700, 70);
    button_play2->setSize(100, 100);
    button_play2->SetSize(100, 100);
    button_play2->SetOnClick([]() {
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSPause>());
        });
    Model* P1Model = ResourceManager::GetInstance()->GetModel(2);
    Texture* P1Texture = ResourceManager::GetInstance()->GetTexture(7);
    Shaders* P1Shader = ResourceManager::GetInstance()->GetShader(0);

    P1 = new Object(P1Model, P1Texture, P1Shader);
    P1->x = 300;
    P1->y = 200;
    P1->set2Dposition(P1->x, P1->y);
    P1->setSize(100, 100);
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(1);
    Texture* texture = ResourceManager::GetInstance()->GetTexture(18);

    spriteAnim = std::make_shared<SpriteAnimation>(model, shader, texture,
        3, // numFrames
        0, // currentFrame
        1, // numActions
        0, // currentAction
        0.15f); // frameTime

    spriteAnim->SetPosition(Vector3(400, 300, 0));
    spriteAnim->SetScale(Vector3(150, 200, 1));

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
    if (spriteAnim->GetCurrentFrame() == 0 ) {
        spriteAnim->SetTexture(ResourceManager::GetInstance()->GetTexture(18));
        spriteAnim->SetNumFrames(3);
        spriteAnim->SetFrameTime(0.1);
    }
    spriteAnim->Update(deltaTime);
    pdltime += deltaTime;
    if (count == 1 && pdltime > 0.3) {
        P1->objTex = ResourceManager::GetInstance()->GetTexture(16);
        count = 2;
        pdltime = 0;
    }
    if (count == 2 && pdltime > 0.3) {
        P1->objTex = ResourceManager::GetInstance()->GetTexture(15);
        count = 3;
        pdltime = 0;
    }
    if (count == 3 && pdltime > 0.3) {
        P1->objTex = ResourceManager::GetInstance()->GetTexture(7);
        count = 1;
        pdltime = 0;
    }
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
    if (keyState['F'])
    {

    spriteAnim->SetTexture(ResourceManager::GetInstance()->GetTexture(19));
    spriteAnim->SetNumFrames(11);
        
    }
    if (keyState['K'])
    {
        dltime += deltaTime;
        if (dltime >= 0.1f)
        {
            int frame = (x++) % 2;
            if (frame == 0)
                P1->objTex = ResourceManager::GetInstance()->GetTexture(13);
            else
                P1->objTex = ResourceManager::GetInstance()->GetTexture(14);

            dltime = 0.0f;
        }
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
    if (button_play2)
    {
        button_play2->Draw();
    }
    if (P1 && spriteAnim)
    {
        P1->Draw();
        spriteAnim->Draw();
    }
    //if (spriteAnim)
    //{
    //    spriteAnim->Draw();
    //}

}
