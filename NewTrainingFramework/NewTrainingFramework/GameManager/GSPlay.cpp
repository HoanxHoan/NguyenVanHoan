#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
#include <algorithm>

GSPlay::GSPlay() {
    x = 0; y = 0; count = 0;
    dltime = 0.05f;
    pdltime = 0.0f;
    Init();
}
GSPlay::~GSPlay() {
    if (P1) {
        delete P1;
        P1 = nullptr;
    }
    if (P2) {
        delete P2;
        P2 = nullptr;
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
        P1->SetVisible(true);
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

   //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(3);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    button_play = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button_play->set2Dposition(850, 70);
    button_play->SetPosition(850, 70);
    button_play->setSize(10, 10);
    button_play->SetSize(10, 10);
    button_play->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    i_objects.push_back(button_play.get());
    Model* btnModel2 = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(6);
    Shaders* btnShader2 = ResourceManager::GetInstance()->GetShader(0);
    button_play2 = std::make_shared<GameButton>(btnModel2, btnTexture2, btnShader2);
    button_play2->set2Dposition(700, 70);
    button_play2->SetPosition(700, 70);
    button_play2->setSize(10, 10);
    button_play2->SetSize(10, 10);
    button_play2->SetOnClick([]() {
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSPause>());

        });
    i_objects.push_back(button_play2.get());
    Model* P1Model = ResourceManager::GetInstance()->GetModel(2);
    Texture* P1Texture = ResourceManager::GetInstance()->GetTexture(7);
    Shaders* P1Shader = ResourceManager::GetInstance()->GetShader(0);

    P1 = new Object(P1Model, P1Texture, P1Shader);
    P1->x = 440;
    P1->y = 300;
    P1->set2Dposition(P1->x, P1->y);
    P1->setSize(10, 10);
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
    spriteAnim->SetScale(Vector3(15, 20, 1));
    P2 = new Object(P1Model, P1Texture, P1Shader);
    P2->x = 450;
    P2->y = 340;
    P2->set2Dposition(P2->x, P2->y);
    P2->setSize(15, 20);
    i_objects.push_back(P1);
    i_objects.push_back(P2);
    Model* quadModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* blackTex = ResourceManager::GetInstance()->GetTexture(20);
    Shaders* overlayShader = ResourceManager::GetInstance()->GetShader(2);

    overlay = std::make_shared<Overlay>(quadModel, blackTex, overlayShader);
    overlay->SetOverlayPosition(P1->x, P1->y);
    overlay->SetOverlaySize((float)Globals::screenWidth, (float)Globals::screenHeight);

    Texture* cirTex = ResourceManager::GetInstance()->GetTexture(19);
    Shaders* cirShader = ResourceManager::GetInstance()->GetShader(2);
    //overlay2 = std::make_shared<Overlay>(quadModel, cirTex, cirShader);
    //overlay2->SetOverlayPosition(P1->x, P1->y);
    //overlay2->SetOverlaySize(30, 30);
    //overlay3 = std::make_shared<Overlay>(quadModel, cirTex, cirShader);
    //overlay3->SetOverlayPosition(P2->x, P2->y);
    //overlay3->SetOverlaySize(30, 30);
    lights.push_back(Vector2(P1->x, P1->y));

    overlay->SetLights(lights);
    overlay->SetLightRadius(0.5f);
    overlay->SetLightSoftness(100.0f);
    overlay->alpha = 1.0f;
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
    lights.clear(); 
    lights.push_back(Vector2(480, 360)); 
    lights.emplace_back(Vector2(480+(P2->x-P1->x), 360.0-(P2->y-P1->y))); 
    overlay->SetLights(lights);
    dltime += deltaTime;
    overlay->GetgameTime(dltime);
    overlay->alpha += dltime * deltaTime;

    //if (overlay->alpha >0.9) dltime = -0.05f;
    //if (overlay->alpha < 0.0) dltime = 0.05f;
    overlay->SetOverlayPosition(P1->x, P1->y);

    std::sort(i_objects.begin(), i_objects.end(), [](Object* a, Object* b) {
        return a->y < b->y;
        });
    Camera::GetInstance()->Follow(P1->x, P1->y);
    if (spriteAnim->GetCurrentFrame() == 0) {
        spriteAnim->SetTexture(ResourceManager::GetInstance()->GetTexture(18));
        spriteAnim->SetNumFrames(3);
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
    if (keyState['W'])
    {
        if (P1->y >= -1000) { P1->y -= 1; }
        //if (P1->CheckCollision(P2)){ P1->y += 10; }
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['D'])
    {
        if (P1->x <= 800) { P1->x += 1; }
        //if (P1->CheckCollision(P2)) { P1->x -= 10; }
        P1->objTex = ResourceManager::GetInstance()->GetTexture(8);
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['A'])
    {
        if (P1->x >= -1000) { P1->x -= 1; }
        //if (P1->CheckCollision(P2)) { P1->x += 10; }
        P1->objTex = ResourceManager::GetInstance()->GetTexture(9);
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['S'])
    {
        if (P1->y <= 600) { P1->y += 1; }
        //if (P1->CheckCollision(P2)) { P1->y -= 10; }
        P1->objTex = ResourceManager::GetInstance()->GetTexture(8);
        P1->set2Dposition(P1->x, P1->y);
    }
    if (keyState['F'])
    {

        spriteAnim->SetTexture(ResourceManager::GetInstance()->GetTexture(19));
        spriteAnim->SetNumFrames(11);
        P1->SetVisible(false);
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
    //if (spriteAnim)
    //{
    //    spriteAnim->Draw();
    //}
   //if (overlay2) {
   //    overlay2->Draw();
   //}
   //if (overlay3) {
   //    overlay3->Draw();
   //}
    for (auto obj : i_objects) { obj->Draw(); }
    if (overlay) {
        overlay->Draw();
    }
}