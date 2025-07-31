#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
#include <algorithm>
#include "../tinyxml2.h"
GSPlay::GSPlay() {
    x = 0; y = 0; count = 0;
    dltime = 0.05f;
    pdltime = 0.0f;
    Init();
}
GSPlay::~GSPlay() {

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
        if (spriteAnim->GetCurrentFrame() == 4) {
            spriteAnim->SetFrameTime(0.7);
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
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(1);
    Texture* texture = ResourceManager::GetInstance()->GetTexture(17);

    spriteAnim = std::make_shared<SpriteAnimation>(model, shader, texture,
        3, // numFrames
        0, // currentFrame
        1, // numActions
        0, // currentAction
        0.1f); // frameTime

    spriteAnim->SetPosition(Vector3(400, 300, 0));
    spriteAnim->SetScale(Vector3(15, 20, 0));
    i_objects.push_back(spriteAnim.get());
    Model* quadModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* blackTex = ResourceManager::GetInstance()->GetTexture(20);
    Shaders* overlayShader = ResourceManager::GetInstance()->GetShader(2);

    overlay = std::make_shared<Overlay>(quadModel, blackTex, overlayShader);
    overlay->SetOverlayPosition(spriteAnim->x, spriteAnim->y);
    overlay->SetOverlaySize((float)Globals::screenWidth, (float)Globals::screenHeight);

    Texture* cirTex = ResourceManager::GetInstance()->GetTexture(19);
    Shaders* cirShader = ResourceManager::GetInstance()->GetShader(2);

    lights.push_back(Vector2(spriteAnim->x, spriteAnim->y));

    overlay->SetLights(lights);
    overlay->SetLightRadius(1.0f);
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
    //lights.emplace_back(Vector2(480+(P2->x- spriteAnim->x), 360.0-(P2->y- spriteAnim->y)));
    overlay->SetLights(lights);
    dltime += deltaTime;
    overlay->GetgameTime(dltime);
    overlay->SetOverlayPosition(spriteAnim->x, spriteAnim->y);

    std::sort(i_objects.begin(), i_objects.end(), [](Object* a, Object* b) {
        return a->y < b->y;
        });
    Camera::GetInstance()->Follow(spriteAnim->x, spriteAnim->y);
    if (spriteAnim->GetCurrentFrame() == 0) {
        spriteAnim->SetTexture(ResourceManager::GetInstance()->GetTexture(17));
        spriteAnim->SetNumFrames(3);
    }
    spriteAnim->Update(deltaTime);
    pdltime += deltaTime;
    if (keyState['W'])
    { 
        if (spriteAnim->y >= -1000 ) { spriteAnim->y -= 1; }
        spriteAnim->SetPosition(Vector3(spriteAnim->x, spriteAnim->y,0));

    }
    if (keyState['D'])
    {
        if (spriteAnim->x <= 800) { spriteAnim->x += 1; }
        if (spriteAnim->CheckCollision(P2)) { spriteAnim->x -= 1; }

        spriteAnim->SetPosition(Vector3(spriteAnim->x, spriteAnim->y, 0));

    }
    if (keyState['A'])
    {
        if (spriteAnim->x >= -1000 ) { spriteAnim->x -= 1; }
        if (spriteAnim->CheckCollision(P2)) { spriteAnim->x += 1; }
        spriteAnim->SetPosition(Vector3(spriteAnim->x, spriteAnim->y, 0));

    }
    if (keyState['S'])
    {
        if (spriteAnim->y <= 600 ) { spriteAnim->y += 1; }
        if (spriteAnim->CheckCollision(P2)) { spriteAnim->y -= 1; }
        spriteAnim->SetPosition(Vector3(spriteAnim->x, spriteAnim->y, 0));

    }
    if (keyState['F'])
    {
        spriteAnim->SetTexture(ResourceManager::GetInstance()->GetTexture(16));
        spriteAnim->SetNumFrames(11);
    }

    if (keyState['K'])
    {

    }
    if (keyState['J'])
    {

    }

}

void GSPlay::Draw()
{

    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(2);
    }
    for (auto obj : i_objects) { obj->Draw(); }
    if (overlay) {
        overlay->Draw();
    }
}