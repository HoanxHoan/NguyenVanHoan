#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
#include <algorithm>
float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
bool GSPlay::IsWaterTile(int xPixel, int yPixel) {
    int col = xPixel / tileWidth;
    int row = yPixel / tileHeight;
    int index = row * mapWidth + col;

    if (index >= 0 && index < waterTiles.size()) {
        return waterTiles[index] != 0;
    }
    return false;
}
GSPlay::GSPlay() {
    x = 0; y = 0;
    dltime = 0.05f;
    actiontime = 0.0f;
    action = 0;
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

    }
    else {

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
    doc.LoadFile("../Resources/Textures/Map.tmx");

    auto mapNode = doc.FirstChildElement("map");

    mapWidth = mapNode->IntAttribute("width");      
    mapHeight = mapNode->IntAttribute("height");    
    tileWidth = mapNode->IntAttribute("tilewidth"); 
    tileHeight = mapNode->IntAttribute("tileheight");

    mapPixelWidth = mapWidth * tileWidth;
    mapPixelHeight = mapHeight * tileHeight;
    auto layer = mapNode->FirstChildElement("layer");
    while (layer) {
        std::string layerName = layer->Attribute("name");
        if (layerName == "Water") {
            const char* data = layer->FirstChildElement("data")->GetText();
            std::stringstream ss(data);
            std::string gid;
            while (std::getline(ss, gid, ',')) {
                waterTiles.push_back(std::stoi(gid));
            }
            break;
        }
        layer = layer->NextSiblingElement("layer");
    }
    //Camera::GetInstance()->UpdateOrthographic(0.0f, Globals::screenWidth, Globals::screenHeight, 0.0f);

    Model* btnModel2 = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(6);
    Shaders* btnShader2 = ResourceManager::GetInstance()->GetShader(0);
    button_play2 = std::make_shared<GameButton>(btnModel2, btnTexture2, btnShader2);
    button_play2->SetPosition(700, 70);
    button_play2->SetSize(10, 10);
    button_play2->SetOnClick([]() {
        GameStateMachine::GetInstance()->PushState(std::make_unique<GSPause>());

        });
    i_objects.push_back(button_play2.get());
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(1);
    Texture* texture = ResourceManager::GetInstance()->GetTexture(8);

    P1 = std::make_shared<SpriteAnimation>(model, shader, texture,
        4, // numFrames
        0, // currentFrame
        1, // numActions
        0, // currentAction
        0.1f); // frameTime

    P1->SetPosition(Vector3(0, 0, 0));
    P1->SetScale(Vector3(15, 20, 0));
    i_objects.push_back(P1.get());
    Model* quadModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* blackTex = ResourceManager::GetInstance()->GetTexture(20);
    Shaders* overlayShader = ResourceManager::GetInstance()->GetShader(2);

    overlay = std::make_shared<Overlay>(quadModel, blackTex, overlayShader);
    overlay->SetOverlayPosition(P1->x, P1->y);
    overlay->SetOverlaySize((float)Globals::screenWidth, (float)Globals::screenHeight);

    Texture* cirTex = ResourceManager::GetInstance()->GetTexture(19);
    Shaders* cirShader = ResourceManager::GetInstance()->GetShader(2);

    lights.push_back(Vector2(P1->x, P1->y));

    overlay->SetLights(lights);
    overlay->SetLightRadius(15.0f);
    overlay->SetLightSoftness(100.0f);

    Texture* btexture = ResourceManager::GetInstance()->GetTexture(13);
    bonfire = std::make_shared<Building>(model, shader, btexture, 4, 0, 1, 0, 0.1f);
    bonfire->SetPosition(Vector3(20, 10, 0));
    bonfire->SetScale(Vector3(5, 5, 0));
    i_bonfire.push_back(bonfire.get());
    Texture* ftexture = ResourceManager::GetInstance()->GetTexture(14);
    fire = std::make_shared<Building>(model, shader, ftexture, 4, 0, 1, 0, 0.1f);
    fire->SetPosition(Vector3(20, bonfire->y - 1, 0));
    fire->SetScale(Vector3(5, 8, 0));
    i_bonfire.push_back(fire.get());
    Texture* treetexture = ResourceManager::GetInstance()->GetTexture(18);
    tree = std::make_shared<Environment>(model, shader, treetexture, 1, 0, 1, 0, 0.1f);
    tree->SetPosition(Vector3(50, 10, 0));
    tree->SetScale(Vector3(10, 20, 0));
    i_objects.push_back(tree.get());
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(3);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    button_play = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button_play->SetPosition(850, 70);
    button_play->SetSize(90, 90);
    button_play->setSize(10,10);
    button_play->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    
    Texture* orgTexture = ResourceManager::GetInstance()->GetTexture(22);
    org = std::make_shared<Enemy>(model, shader, orgTexture, 6, 0, 1, 0, 0.1f);
    org->SetPosition(Vector3(70, 10, 0));
    org->SetScale(Vector3(14, 16, 0));
    i_objects.push_back(org.get());
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
    org->moveTo(P1->x, P1->y);
    button_play->set2Dposition(P1->x + 39, P1->y - 41);
    if (action == 1 ) {
        actiontime += deltaTime;
        if (actiontime >= 0.76) {
            action = 0;
            actiontime = 0;
            tree->EndCutTree();
        }
    }
    if (action == 0) {
        P1->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
        P1->SetNumFrames(4);
        switch (count) {
        case 1:P1->SetTexture(ResourceManager::GetInstance()->GetTexture(9)); break;
        case 2: { 
            P1->SetTexture(ResourceManager::GetInstance()->GetTexture(7));
            break; }
        case 3:P1->SetTexture(ResourceManager::GetInstance()->GetTexture(8)); break;
        case 4: {
            P1->SetTexture(ResourceManager::GetInstance()->GetTexture(7));
            P1->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
            break;
        }
        }
    }

    P1->x = clamp(P1->x, 0.0f, (float)(mapPixelWidth - P1->width));
    P1->y = clamp(P1->y, 0.0f, (float)(mapPixelHeight - P1->height));
    lights.clear(); 
    lights.push_back(Vector2(480, 360)); 
    lights.emplace_back(Vector2(480+(bonfire->x- P1->x), 360.0-(bonfire->y- P1->y)));
    overlay->SetLights(lights);
    dltime += deltaTime;
    overlay->GetgameTime(dltime);
    overlay->SetOverlayPosition(P1->x, P1->y);

    std::sort(i_objects.begin(), i_objects.end(), [](Object* a, Object* b) {
        return a->y < b->y;
        });
    Camera::GetInstance()->Follow(P1->x, P1->y);
    for (auto obj : i_objects) {
        obj->Update(deltaTime); 
    }
    for (auto obj : i_bonfire) {
        obj->Update(deltaTime);
    }
    if (keyState['W']) {
        if (action == 0) {
            count = 1;
            P1->SetTexture(ResourceManager::GetInstance()->GetTexture(5));
            P1->SetNumFrames(6);
            float newY = P1->y - 10 * deltaTime;
            if (!IsWaterTile(P1->x, newY)) {
                P1->y = newY;
                P1->SetPosition(Vector3(P1->x, P1->y, 0));
            }
        }
    }
    if (keyState['D']) {
        if (action == 0) {
            if (count == 4)P1->SetRotation(Vector3(0, 360 * DEG2RAD, 0));
            count = 2;
            P1->SetTexture(ResourceManager::GetInstance()->GetTexture(0));
            P1->SetNumFrames(6);
            float newX = P1->x + 10 * deltaTime;
            if (!IsWaterTile(newX, P1->y)) {
                P1->x = newX;
                P1->SetPosition(Vector3(P1->x, P1->y, 0));
            }
        }
    }
    if (keyState['A']) {
        if (action == 0) {
            count = 4;
            P1->SetTexture(ResourceManager::GetInstance()->GetTexture(0));
            P1->SetNumFrames(6);
            P1->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
            float newX = P1->x - 10 * deltaTime;
            if (!IsWaterTile(newX, P1->y)) {
                P1->x = newX;
                P1->SetPosition(Vector3(P1->x, P1->y, 0));
            }
        }
    }
    if (keyState['S']) {
        if (action == 0) {
            count = 3;
            P1->SetTexture(ResourceManager::GetInstance()->GetTexture(4));
            P1->SetNumFrames(6);
            float newY = P1->y + 10*deltaTime;
            if (!IsWaterTile(P1->x, newY)) {
                P1->y = newY;
                P1->SetPosition(Vector3(P1->x, P1->y, 0));
            }
        }
    }
    if (keyState['J'])
    {
        if (action == 0) {
            switch (count) {
            case 1: {
                P1->SetTexture(ResourceManager::GetInstance()->GetTexture(12));
                P1->SetNumFrames(8);
                P1->SetCurrentFrame(0);
                action = 1;
                break;
            }
            case 2: {
                P1->SetTexture(ResourceManager::GetInstance()->GetTexture(10));
                P1->SetNumFrames(8);
                P1->SetCurrentFrame(0);
                tree->CutTree();
                action = 1;
                break;
            }
            case 3: {
                P1->SetTexture(ResourceManager::GetInstance()->GetTexture(11));
                P1->SetNumFrames(8);
                P1->SetCurrentFrame(0);
                action = 1;
                break;
            }
            case 4: {
                P1->SetTexture(ResourceManager::GetInstance()->GetTexture(10));
                P1->SetNumFrames(8);
                P1->SetCurrentFrame(0);
                P1->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
                action = 1;
                break;
            }
            }
        } 

    }

}

void GSPlay::Draw()
{

    if (Scene::GetInstance())
    {
        Scene::GetInstance()->Render(2);
    }
    for (auto obj : i_bonfire) { obj->Draw(); }
    for (auto obj : i_objects) { obj->Draw(); }

    if (overlay) {
        overlay->Draw();
    }
    button_play->Draw();
}

