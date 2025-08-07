#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
#include <algorithm>

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
Vector3 GSPlay::GetRandomValidPosition() {
    int maxAttempts = 1000;

    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        int tileX = rand() % mapWidth;
        int tileY = rand() % mapHeight;

        float x = tileX * 16;
        float y = tileY * 16;

        if (!IsWaterTile(x, y)) {
            return Vector3(x, y, 0);
        }
    }
    return Vector3(0, 0, 0);
}
Vector3 GSPlay::GenerateRandomValidPositionAvoidCollision(const std::vector<std::shared_ptr<Object>>& others) {
    int maxAttempts = 1000;
    for (int i = 0; i < maxAttempts; ++i) {
        int col = rand() % (mapWidth);
        int row = rand() % (mapHeight);

        float x = col * tileWidth;
        float y = row * tileHeight ;

        if (!IsWaterTile(x, y)&& !IsWaterTile(x+1, y)&& !IsWaterTile(x-1, y)&& !IsWaterTile(x, y+1)&& !IsWaterTile(x, y-1)) {
            Object temp;
            temp.setSize(100, 100);
            temp.set2Dposition(x, y);
            bool collision = false;
            for (auto& obj : others) {
                if (temp.CheckCollision(obj.get())){
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                return Vector3(x, y, 0);
            }
        }
    }
    return Vector3(0, 0, 0);
}

bool GSPlay::IsWaterTile(int xPixel, int yPixel) {
    int col = xPixel / (tileWidth);
    int row = yPixel / (tileHeight);
    int index = row * mapWidth + col;

    if (index >= 0 && index < waterTiles.size()) {
        return waterTiles[index] != 0;
    }
    return false;
}
GSPlay::GSPlay() {
    srand(time(NULL));
    x = 0; y = 0;
    dltime = 0.05f;
    actiontime = 0.0f;
    uidltime = 0.00f;
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
    for (auto slot : inventorySlots) {
        if (slot->HandleTouchEvents(x, y, isClick)) {
            printf("Slot clicked at position (%d, %d)\n", x, y);
            return;
        }
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

    //Model* btnModel2 = ResourceManager::GetInstance()->GetModel(2);
    //Texture* btnTexture2 = ResourceManager::GetInstance()->GetTexture(6);
    //Shaders* btnShader2 = ResourceManager::GetInstance()->GetShader(0);
    //button_play2 = std::make_shared<GameButton>(btnModel2, btnTexture2, btnShader2);
    //button_play2->SetPosition(700, 70);
    //button_play2->SetSize(10, 10);
    //button_play2->SetOnClick([]() {
    //    GameStateMachine::GetInstance()->PushState(std::make_unique<GSPause>());

    //    });
    //i_objects.push_back(button_play2.get());
    Model* btnModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* btnTexture = ResourceManager::GetInstance()->GetTexture(3);
    Shaders* btnShader = ResourceManager::GetInstance()->GetShader(0);
    Model* model = ResourceManager::GetInstance()->GetModel(2);
    Shaders* shader = ResourceManager::GetInstance()->GetShader(1);
    Texture* texture = ResourceManager::GetInstance()->GetTexture(8);

    P1 = std::make_shared<Player>(model, shader, texture,
        4, // numFrames
        0, // currentFrame
        1, // numActions
        0, // currentAction
        0.1f); // frameTime

    P1->SetPosition(Vector3(200, 200, 0));
    P1->SetScale(Vector3(30, 40, 0));
    i_objects.push_back(P1);
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
    //bonfire
    Texture* btexture = ResourceManager::GetInstance()->GetTexture(13);
    bonfire = std::make_shared<Building>(model, shader, btexture, 4, 0, 1, 0, 0.1f);
    bonfire->SetPosition(Vector3(300, 200, 0));
    bonfire->SetScale(Vector3(10, 10, 0));
    i_bonfire.push_back(bonfire.get());
    envi_objects.push_back(bonfire);
    Texture* ftexture = ResourceManager::GetInstance()->GetTexture(14);
    fire = std::make_shared<Building>(model, shader, ftexture, 4, 0, 1, 0, 0.1f);
    fire->SetPosition(Vector3(300, bonfire->y - 2, 0));
    fire->SetScale(Vector3(10, 16, 0));
    i_bonfire.push_back(fire.get());
    envi_objects.push_back(fire);
    //Tree
    Texture* treetexture = ResourceManager::GetInstance()->GetTexture(18);
    for (int i = 0; i < 150; ++i) {
        tree = std::make_shared<Tree>(model, shader, treetexture, 1, 0, 1, 0, 0.1f);
        tree->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        tree->SetScale(Vector3(40, 80, 0));
        i_objects.push_back(tree);
        envi_objects.push_back(tree);
    }
    //rocks
    Texture* stonetexture1 = ResourceManager::GetInstance()->GetTexture(28);
    Texture* stonetexture2 = ResourceManager::GetInstance()->GetTexture(29);
    Texture* stonetexture3 = ResourceManager::GetInstance()->GetTexture(30);
    for (int i = 0; i < 20; ++i) {
        stone = std::make_shared<Stone>(model, shader, stonetexture1, 1, 0, 1, 0, 0.1f);
        stone->type = 1;
        stone->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        stone->SetScale(Vector3(20, 40, 0));
        stone->setSize(20, 30);
        i_objects.push_back(stone);
        envi_objects.push_back(stone);
    }
    for (int i = 0; i < 30; ++i) {
        stone = std::make_shared<Stone>(model, shader, stonetexture2, 1, 0, 1, 0, 0.1f);
        stone->type = 2;
        stone->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        stone->SetScale(Vector3(30, 40, 0));
        stone->setSize(40, 20);
        i_objects.push_back(stone);
        envi_objects.push_back(stone);
    }
    //bush
    Texture* bushtexture1 = ResourceManager::GetInstance()->GetTexture(35);
    Texture* bushtexture2 = ResourceManager::GetInstance()->GetTexture(36);
    for (int i = 0; i < 50; ++i) {
        bush = std::make_shared<Bush>(model, shader, bushtexture1, 1, 0, 1, 0, 0.1f);
        bush->type = 1;
        bush->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        bush->SetScale(Vector3(15, 15, 0));
        bush->setSize(10, 10);
        i_objects.push_back(bush);
        envi_objects.push_back(bush);
    }
    for (int i = 0; i < 50; ++i) {
        bush = std::make_shared<Bush>(model, shader, bushtexture2, 1, 0, 1, 0, 0.1f);
        bush->type = 2;
        bush->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        bush->SetScale(Vector3(15, 15, 0));
        bush->setSize(10, 10);
        i_objects.push_back(bush);
        envi_objects.push_back(bush);
    }
    //animal
    Texture* deertexture = ResourceManager::GetInstance()->GetTexture(39);
    Texture* boartexture = ResourceManager::GetInstance()->GetTexture(43);
    for (int i = 0; i < 30; ++i) {
        animal = std::make_shared<Animal>(model, shader, deertexture, 4, 0, 1, 0, 0.1f);
        animal->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        animal->type = 1;
        animal->getwaterTiles(waterTiles);
        animal->getObjectList(&i_objects);
        animal->SetScale(Vector3(15, 15, 0));
        animal->setSize(25, 20);
        i_objects.push_back(animal);
        envi_objects.push_back(animal);
    }
    for (int i = 0; i < 30; ++i) {
    animal = std::make_shared<Animal>(model, shader, boartexture, 4, 0, 1, 0, 0.1f);
    animal->type = 2;
    animal->hp = 4;
    animal->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
    animal->getwaterTiles(waterTiles);
    animal->getObjectList(&i_objects);
    animal->SetScale(Vector3(20, 20, 0));
    animal->setSize(30, 25);
    i_objects.push_back(animal);
    envi_objects.push_back(animal);
    }
	Texture* inventoryTexture = ResourceManager::GetInstance()->GetTexture(34);
	inventory = std::make_shared<Building>(model, shader, inventoryTexture, 1, 0, 1, 0, 0.1f);
	inventory->SetPosition(Vector3(P1->x, P1->y, 0));
    inventory->SetScale(Vector3(200, 110, 0));
    inventory->SetVisible(false);

    Texture* slotTexture = ResourceManager::GetInstance()->GetTexture(3); 
    inv = std::make_shared<InventorySlots>(
        model, slotTexture, shader
    );

        for (int i = 0; i < 30; ++i) {
            slot = std::make_shared<Slot>(model, slotTexture, btnShader);
            printf("Slot %d created\n", i);
            int cols = 10;
            int row = i / cols;
            int col = i % cols;

            float offsetX = -90 + col * 20.0f;
            float offsetY = 40 - row * 20.0f;
			slot->setSize(10, 10);
            slot->SetSize(20, 20);
            slot->SetPosition(480 + offsetX * 4.8 , 360 + offsetY * 3.6);
            //slot->SetPosition(P1->x, P1->y);
            slot->set2Dposition(P1->x + offsetX, P1->y + offsetY);
            slot->SetOnClick([]() {
                printf("Slot clicked\n");
                
				});
            //inv->AddSlot(slot);
            inventorySlots.push_back(slot);
            
        
    }

    //tree = std::make_shared<Environment>(model, shader, treetexture, 1, 0, 1, 0, 0.1f);
    //tree->SetPosition(Vector3(50, 10, 0));
    //tree->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
    //printf("%f,%f,%f\n", GenerateRandomValidPositionAvoidCollision(i_objects).x, GenerateRandomValidPositionAvoidCollision(i_objects).y, GenerateRandomValidPositionAvoidCollision(i_objects).z);
    //tree->SetScale(Vector3(10, 20, 0));
    //i_objects.push_back(tree.get());
    //envi_objects.push_back(tree.get());

    button_play = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button_play->SetPosition(850, 80);
    button_play->SetSize(90, 80);
    button_play->setSize(20,20);
    button_play->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    //enermy
    Texture* orgTexture = ResourceManager::GetInstance()->GetTexture(22);
    org = std::make_shared<Enemy>(model, shader, orgTexture, 6, 0, 1, 0, 0.1f);
    org->SetPosition(Vector3(400, 400, 0));
    org->SetScale(Vector3(28, 32, 0));
    i_objects.push_back(org);
    enermy_objects.push_back(org.get());
    temp = *P1;
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
	uidltime += deltaTime;
    inventory->SetPosition(Vector3(P1->x, P1->y, 0));
    for (auto& slot : inventorySlots) {
        int cols = 10;
        int row = i / cols;
        int col = i % cols;
        float offsetX = -90 + col * 20.0f;
        float offsetY = 40 - row * 20.0f;
        slot->set2Dposition(P1->x + offsetX, P1->y + offsetY);
        i++;
        if (i >= 30) {
            i = 0;
        }
    }
	//inv->Update(P1->x, P1->y, deltaTime);

	
    //org->moveTo(P1->x, P1->y);
    //for (size_t i = 0; i < enermy_objects.size(); ++i) {
    //    if (P1->CheckCollision(enermy_objects[i])) {
    //        enermyCollision = true;
    //        break;
    //    }
    //}
    //if (enermyCollision) {
    //    org->Dead(); 
    //    enermyCollision = false;
    //}
    button_play->set2Dposition(P1->x + 78, P1->y - 80);
    if (action == 1 ) {
        actiontime += deltaTime;
        if (actiontime >= 0.76) {
            action = 0;
            actiontime = 0;
        }
    }
    if (action == 0) {
        P1->Idle(count);
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

    std::sort(i_objects.begin(), i_objects.end(),
        [](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
            return a->y < b->y; 
        });
    Camera::GetInstance()->Follow(P1->x, P1->y);
    for (auto& obj : i_objects) {
        obj.get()->Update(deltaTime);
    }
    for (auto obj : i_bonfire) {
        obj->Update(deltaTime);
    }
    if (keyState['W']) {
        if (action == 0) {
            hasCollision = false;
            count = 1;
            float newY = P1->y - 300 * deltaTime;
            temp = *P1;
            temp.y = newY;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision ) {
                P1->MoveUp(newY, IsWaterTile(P1->x, newY));
            }
        }
    }
    if (keyState['D']) {
        if (action == 0) {
            hasCollision = false;
            count = 2;
            float newX = P1->x + 300 * deltaTime;
            temp = *P1;
            temp.x = newX;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision) {
                P1->MoveRight(newX, IsWaterTile(newX, P1->y));
            }
        }
    }
    if (keyState['A']) {
        if (action == 0) {
            hasCollision = false;
            count = 4;
            float newX = P1->x - 300 * deltaTime;
            temp = *P1;
            temp.x = newX;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision) {
                P1->MoveLeft(newX, IsWaterTile(newX, P1->y));
            }
        }
    }
    if (keyState['S']) {
        if (action == 0) {
            hasCollision = false;
            count = 3;
            float newY = P1->y + 300 * deltaTime;
            temp.y = newY;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision) {
                P1->MoveDown(newY, IsWaterTile(P1->x, newY));
            }
        }
    }
    if (keyState['J'])
    {    
        P1->Crush(action, count);
        action = 1;
        for (auto& obj : envi_objects) {
            if (P1->GetHitbox(count)->CheckCollisionStone(obj.get()))
            {
                if (auto env = dynamic_cast<Stone*>(obj.get())) {
                    env->Crush();
                }
            }
        }
    }
    if (keyState['K'])
    {
        P1->Slice(action, count);
        action = 1;
        for (auto& obj : envi_objects) {        
                if (auto env = dynamic_cast<Tree*>(obj.get())) {
                    if (P1->GetHitbox(count)->CheckCollisionTree(obj.get()))
                    {
                        env->CutTree();
                    }
                }
                if (auto env = dynamic_cast<Bush*>(obj.get())) {
                    if (P1->GetHitbox(count)->CheckCollision(obj.get()))
                    {
                        env->Cut();
                    }
                }
                if (auto env = dynamic_cast<Animal*>(obj.get())) {
                    if (P1->GetHitbox(count)->CheckCollision(obj.get()))
                    {
                        env->onHit();
                    }
                }
            
        }
        if (P1->GetHitbox(count)->CheckCollisionEnermy(org.get()))
        {
            org->Dead();
        }
    }
    if (keyState['E'])
    {
        if (uidltime >= 0.2f) {
            uidltime = 0.0f;
            inventory->SetVisible(!inventory->IsVisible());
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
	inventory->Draw();
    if (inventory->IsVisible()) {
        for (auto& sl : inventorySlots) {
            sl->Draw();
        }
    }
    button_play->Draw();
    
}

