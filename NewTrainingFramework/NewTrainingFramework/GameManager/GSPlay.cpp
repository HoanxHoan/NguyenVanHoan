#include "../stdafx.h" 
#include "GSPlay.h"
#include <iostream>
#include <algorithm>
#include "ItemDb.h"
#include "CraftingRecipeDB.h"


std::shared_ptr<CraftingUI> GSPlay::m_craftingUI = nullptr;
std::string GSPlay::m_recipeId = "";

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
        float y = row * tileHeight;

        if (!IsWaterTile(x, y) && !IsWaterTile(x + 1, y) && !IsWaterTile(x - 1, y) && !IsWaterTile(x, y + 1) && !IsWaterTile(x, y - 1)) {
            Object temp;
            temp.setSize(100, 100);
            temp.set2Dposition(x, y);
            bool collision = false;
            for (auto& obj : others) {
                if (temp.CheckCollision(obj.get())) {
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
    count = 3;
    x = 0; y = 0;
    dltime = 0.05f;
    actiontime = 0.0f;
    uidltime = 0.00f;
    action = 0;
    onhit = false;
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
        isWalk = false;
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
    if(inventory->IsVisible()){
        for (auto slot : inventorySlots) {
            if (slot->HandleTouchEvents(x, y, isClick)) {
                printf("Slot clicked at position (%d, %d)\n", x, y);
                return;
            }
        }
        for (auto slot : hotbar) {
            if (slot->HandleTouchEvents(x, y, isClick)) {
                printf("Hotbar slot clicked at position (%d, %d)\n", x, y);
                return;
            }
        }
        for (auto slot : m_craftingSlots) {
            if (slot->HandleTouchEvents(x, y, isClick)) {
                printf("Hotbar slot clicked at position (%d, %d)\n", x, y);
                return;
            }
        }
    }

}

void GSPlay::ReloadCraftingSlots() {

    auto craftableIds = m_craftingUI->GetCraftableItems();
    Model* slotModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* slotTexture = ResourceManager::GetInstance()->GetTexture(70);
    Shaders* slotShader = ResourceManager::GetInstance()->GetShader(0);

    m_craftingSlots.clear();

    int cols = 10;
    int slotIndex = 0; // Dùng slotIndex riêng cho slot hiện tại vì có thể ko tạo đủ craftableIds.size()
    for (size_t i = 0; i < craftableIds.size(); ++i) {
        const std::string& recipeId = craftableIds[i];

        // Chỉ tạo slot nếu đủ nguyên liệu để craft
        if (!m_craftingUI->CanCraft(recipeId)) {
            continue; // Bỏ qua nếu không đủ nguyên liệu
        }

        auto slot = std::make_shared<Slot>(slotModel, slotTexture, slotShader);

        slot->SetSlotType(SlotType::CRAFTING);
        slot->SetSlotIndex(slotIndex); // dùng slotIndex vì i có thể bỏ qua
        slot->SetOwnerCrafting(playerInventory.get());

        auto item = std::make_shared<Item>(recipeId, 1);
        slot->SetItem(item);

        int row = slotIndex / cols;
        int col = slotIndex % cols;

        float offsetX = -70 + col * 15.0f;
        float offsetY = row * 15.0f - 90;

        slot->setSize(13, 13);
        slot->SetSize(30, 30);
        slot->userOffsetX = offsetX;
        slot->userOffsetY = offsetY;
        slot->SetPosition(480 + offsetX * 4.8, 360 + offsetY * 3.6);
        slot->SetChildPosition(P1->x + offsetX, P1->y + offsetY);
        slot->set2Dposition(P1->x + offsetX, P1->y + offsetY);

        slot->TextPosition(480 + offsetX * 4.8, 360 + offsetY * 3.6);
        // NOTE: Để lambda truy cập đúng m_recipeId và m_craftingUI, cần capture [this, recipeId]
        slot->SetOnClick([]() {
            printf("Crafting item with ID: %s\n", m_recipeId.c_str());
            m_craftingUI->CraftItem(m_recipeId);

            });

        m_craftingSlots.push_back(slot);
        slotIndex++;
    }
}


bool GSPlay::Init()
{
    PlayerInventory::GetInstance()->AddStartingItems();
    printf("----------------------------------\n");
    ItemDB::GetInstance()->LoadDB("../Resources/ItemDb.txt");
    printf("----------------------------------\n");
	CraftingRecipeDB::GetInstance()->LoadFromFile("../Resources/RecipeDb.txt");
	CraftingRecipeDB::GetInstance()->PrintAll();   
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
    P1->setSize(15, 20);
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
    //crystal
    Texture* crystal = ResourceManager::GetInstance()->GetTexture(74);
    stone = std::make_shared<Stone>(model, shader, crystal, 1, 0, 1, 0, 0.1f);
    stone->type = 3;
    stone->SetPosition(Vector3(80, 160, 0));
    stone->SetScale(Vector3(30, 40, 0));
    stone->setSize(40, 20);
    stone->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
    i_objects.push_back(stone);
    envi_objects.push_back(stone);

    stone = std::make_shared<Stone>(model, shader, crystal, 1, 0, 1, 0, 0.1f);
    stone->type = 3;
    stone->SetPosition(Vector3(80, 1500, 0));
    stone->SetScale(Vector3(30, 40, 0));
    stone->setSize(40, 20);
    stone->SetRotation(Vector3(0, 180 * DEG2RAD, 0));
    i_objects.push_back(stone);
    envi_objects.push_back(stone);

    stone = std::make_shared<Stone>(model, shader, crystal, 1, 0, 1, 0, 0.1f);
    stone->type = 3;
    stone->SetPosition(Vector3(2730, 160, 0));
    stone->SetScale(Vector3(30, 40, 0));
    stone->setSize(40, 20);
    i_objects.push_back(stone);
    envi_objects.push_back(stone);

    stone = std::make_shared<Stone>(model, shader, crystal, 1, 0, 1, 0, 0.1f);
    stone->type = 3;
    stone->SetPosition(Vector3(2730, 1500, 0));
    stone->SetScale(Vector3(30, 40, 0));
    stone->setSize(40, 20);
    i_objects.push_back(stone);
    envi_objects.push_back(stone);
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
    //hp
    Texture* hpTexture = ResourceManager::GetInstance()->GetTexture(88);
    hp = std::make_shared<Hmp>(model, hpTexture, btnShader);
    hp->SetPosition(P1->x-60, P1->y+75);
    hp->SetSize(40, 10);
    //mp
    Texture* mpTexture = ResourceManager::GetInstance()->GetTexture(101);
    mp = std::make_shared<Hmp>(model, hpTexture, btnShader);
    mp->SetPosition(P1->x + 55, P1->y + 75);
    mp->SetSize(40, 10);
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
    inventory->SetScale(Vector3(160, 70, 0));
    inventory->SetVisible(false);

    Texture* slotTexture = ResourceManager::GetInstance()->GetTexture(69); 
    playerInventory = std::make_shared<PlayerInventory>();
    for (int i = 0; i < 30; ++i) {
        slot = std::make_shared<Slot>(model, slotTexture, btnShader);
        slot->SetSlotType(SlotType::INVENTORY); // 👈 THÊM DÒNG NÀY
        slot->SetSlotIndex(i);
        slot->SetOwnerInventory(PlayerInventory::GetInstance());
        Slot::allInventorySlots.push_back(slot);
        printf("Slot %d created\n", i);
        int cols = 10;
        int row = i / cols;
        int col = i % cols;
        float offsetX = -43 + col * 15.0f;
        float offsetY = 30 - row * 15.0f;
        slot->setSize(13, 13);
        slot->SetSize(30, 30);
        slot->SetPosition(480 + offsetX * 4.8, 360 + offsetY * 3.6);
        slot->SetChildPosition(P1->x + offsetX, P1->y + offsetY);

        slot->TextPosition(480 + offsetX * 4.8, 360 + offsetY * 3.6);
        //slot->SetPosition(P1->x, P1->y);
        slot->set2Dposition(P1->x + offsetX, P1->y + offsetY);

        slot->SetOnClick([]() {
            printf("Slot clicked\n");
            // Xử lý sự kiện khi slot được nhấn
            });
        //inv->AddSlot(slot);
        inventorySlots.push_back(slot);
        // (tuỳ chọn) nếu bạn cần giữ shared_ptr:

    }

    Slot::ClearHotbarSlots();
    Texture* hotbarTexture = ResourceManager::GetInstance()->GetTexture(70);
    for (int i = 0; i < 10; ++i) {
		hotbar_slot = std::make_shared<Slot>(model, hotbarTexture, btnShader);
        hotbar_slot->SetSlotType(SlotType::HOTBAR);
        hotbar_slot->SetSlotIndex(i);
        hotbar_slot->SetOwnerInventory(PlayerInventory::GetInstance());
        hotbar_slot->RegisterHotbarSlot(hotbar_slot.get());
        printf("Hotbar Slot %d created\n", i);
        int cols = 10;
        int col = i % cols;
        float offsetX = -70 + col * 15.0f;
        hotbar_slot->setSize(15, 15);
        hotbar_slot->SetSize(30, 30);
        hotbar_slot->SetPosition(480 + offsetX * 4.8, 683);
        hotbar_slot->SetChildPosition(P1->x + offsetX, P1->y + 90);
        //hotbar_slot->SetPosition(P1->x, P1->y)e;
        hotbar_slot->set2Dposition(P1->x + offsetX, P1->y + 90);
        hotbar_slot->TextPosition(480 + offsetX * 4.8, 683);
        hotbar_slot->SetOnClick([]() {
            printf("Hotbar Slot clicked\n");
            // Xử lý sự kiện khi slot được nhấn
			});
		hotbar.push_back(hotbar_slot);
    }
    Slot::SetCurrentSlot(currentSlot);

    /*m_craftingUI = std::make_shared<CraftingUI>(playerInventory);
    m_craftingUI->InitializeUI();
	m_craftingUI->*/

    m_craftingUI = std::make_shared<CraftingUI>(playerInventory);
    ReloadCraftingSlots();
    // (Tùy chọn) nếu muốn giữ trong GSPlay để Draw()

    PlayerInventory::GetInstance()->InitializeUI(inventorySlots, hotbar, m_craftingSlots);
    //tree = std::make_shared<Environment>(model, shader, treetexture, 1, 0, 1, 0, 0.1f);
    //tree->SetPosition(Vector3(50, 10, 0));
    //tree->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
    //printf("%f,%f,%f\n", GenerateRandomValidPositionAvoidCollision(i_objects).x, GenerateRandomValidPositionAvoidCollision(i_objects).y, GenerateRandomValidPositionAvoidCollision(i_objects).z);
    //tree->SetScale(Vector3(10, 20, 0));
    //i_objects.push_back(tree.get());
    //envi_objects.push_back(tree.get());
	PlayerInventory::GetInstance()->PrintAllSlots();
    button_play = std::make_shared<GameButton>(btnModel, btnTexture, btnShader);
    button_play->SetPosition(850, 80);
    button_play->SetSize(90, 80);
    button_play->setSize(20, 20);
    button_play->SetOnClick([]() {
        GameStateMachine::GetInstance()->PopState();
        });
    //enermy
    Texture* orgTexture = ResourceManager::GetInstance()->GetTexture(22);
    Texture* skeletonTexture = ResourceManager::GetInstance()->GetTexture(47);
    for (int i = 0; i < 10; ++i) {
        org = std::make_shared<Enemy>(model, shader, orgTexture, 6, 0, 1, 0, 0.1f);
        org->type = 1;
        org->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        org->SetScale(Vector3(15, 20, 0));
        org->setSize(30, 40);
        org->getObjectList(&i_objects);
        i_objects.push_back(org);
        enermy_objects.push_back(org);
        envi_objects.push_back(org);
    }
    for (int i = 0; i < 10; ++i) {
        org = std::make_shared<Enemy>(model, shader, skeletonTexture, 6, 0, 1, 0, 0.1f);
        org->type = 2;
        org->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
        org->SetScale(Vector3(15, 20, 0));
        org->setSize(30, 40);
        org->getObjectList(&i_objects);
        i_objects.push_back(org);
        enermy_objects.push_back(org);
    }
    //Boss
    Texture* SlimeTexture = ResourceManager::GetInstance()->GetTexture(50);
    boss = std::make_shared<Boss>(model, shader, SlimeTexture, 8, 0, 1, 0, 0.1f);
    boss->type = 1;
    boss->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
    boss->SetScale(Vector3(50, 50, 0));
    boss->setSize(70, 70);
    i_objects.push_back(boss);
    Boss_objects.push_back(boss);

    Texture* necroTexture = ResourceManager::GetInstance()->GetTexture(56);
    boss = std::make_shared<Boss>(model, shader, necroTexture, 8, 0, 1, 0, 0.1f);
    boss->type = 2;
    boss->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
    boss->SetScale(Vector3(50, 50, 0));
    boss->setSize(70, 70);
    i_objects.push_back(boss);
    Boss_objects.push_back(boss);

    Texture* StonegiTexture = ResourceManager::GetInstance()->GetTexture(56);
    boss = std::make_shared<Boss>(model, shader, StonegiTexture, 8, 0, 1, 0, 0.1f);
    boss->type = 3;
    boss->SetPosition(GenerateRandomValidPositionAvoidCollision(i_objects));
    boss->SetScale(Vector3(50, 50, 0));
    boss->setSize(80, 80);
    i_objects.push_back(boss);
    Boss_objects.push_back(boss);

    temp = *P1;
    P1->getObjectList(&i_objects);
    return true;
}

void GSPlay::UpdateCraftingSlots() {
    for (const auto& slot : m_craftingSlots) {
        if (slot->GetSlotType() == SlotType::CRAFTING && slot->GetItem() == nullptr) {
            std::string itemId = slot->GetLastItemId();
            if (!itemId.empty()) {
                printf("Crafting item from removed slot: %s\n", itemId.c_str());
                m_craftingUI->CraftItem(itemId);
				PlayerInventory::GetInstance()->PrintAllSlots();
                reloadable = true;
            }
        }
    }

    // Xóa các slot không còn item và là slot crafting
    auto it = std::remove_if(m_craftingSlots.begin(), m_craftingSlots.end(),
        [](const std::shared_ptr<Slot>& slot) {
            return slot->GetSlotType() == SlotType::CRAFTING && slot->GetItem() == nullptr;
        });
    m_craftingSlots.erase(it, m_craftingSlots.end());

    // Cập nhật vị trí các slot crafting còn lại
    

    m_craftingUI->UpdateCraftableList();
}
void GSPlay::Exit()
{
    PlayerInventory::GetInstance()->Reset();
    PlayerInventory::GetInstance()->Destroy();
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
	useitemdltime += deltaTime;
    if(useitemdltime >0.5f && useItem == true) {
        useItem = false;
        useitemdltime = 0;
	}
    mp->UpdateMp(P1->x + 55, P1->y + 75, P1->mp);
    hp->Update(P1->x - 60, P1->y + 75, P1->hp);
    if (PlayerInventory::GetInstance()->updated) {
        PlayerInventory::GetInstance()->InitializeUI(inventorySlots, hotbar, m_craftingSlots);
        PlayerInventory::GetInstance()->updated = false;

    }
    if (isWalk && !wasWalking)
    {
        SoundManager::GetInstance()->PlaySound("walk");
    }

    if (!isWalk && wasWalking)
    {
        SoundManager::GetInstance()->endSound("walk");
    }
    wasWalking = isWalk;
    uidltime += deltaTime;
    inventory->SetPosition(Vector3(P1->x + 25, P1->y+15, 0));
    onhitdltime += deltaTime;
	uidltime += deltaTime;
    if (onhit == true && onhitdltime > 2) {
        onhit = false;
    }
    for (auto& slot : inventorySlots) {
        int cols = 10;
        int row = i / cols;
        int col = i % cols;
        float offsetX = -43 + col * 15.0f;
        float offsetY = 30 - row * 15.0f;
        slot->set2Dposition(P1->x + offsetX, P1->y + offsetY);
        slot->SetChildPosition(P1->x + offsetX, P1->y + offsetY);
        i++;
        if (i >= 30) {
            i = 0;
        }
    }

    for(auto&hotbar_slot : hotbar) {
        int cols = 10;
        int col = i % cols;
        float offsetX = -70 + col * 15.0f;
        hotbar_slot->set2Dposition(P1->x + offsetX, P1->y + 90);
        hotbar_slot->SetChildPosition(P1->x + offsetX, P1->y + 90);
        if (hotbar_slot->IsCurrentSlot()) {
            hotbar_slot->setSize(18, 18);
        }
        else {
            hotbar_slot->setSize(15, 15);
        }
        i++;
        if (i >= 10) {
            i = 0;
        }
	}

    int i = 0;
    for (auto& slot : m_craftingSlots) {
        if (slot->GetSlotType() == SlotType::CRAFTING) {
            int cols = 10;
            int row = i / cols;
            int col = i % cols;

            float offsetX = -70 + col * 15.0f;
            float offsetY = row * 15.0f - 90;

            slot->userOffsetX = offsetX;
            slot->userOffsetY = offsetY;
            slot->set2Dposition(P1->x + offsetX, P1->y + offsetY);
            slot->SetChildPosition(P1->x + offsetX, P1->y + offsetY);
            ++i;
        }
    }


    UpdateCraftingSlots();
    if (reloadable && uidltime >= 0.5) {
        uidltime = 0;
        reloadable = false;
        ReloadCraftingSlots();
	}

    if (PlayerInventory::GetInstance()->reload && uidltime >= 0.5) {
        uidltime = 0;
        PlayerInventory::GetInstance()->reload = false;
        ReloadCraftingSlots();
    }

    // 2. Update vị trí các slot còn lại
    for (auto& slot : m_craftingSlots) {
        slot->set2Dposition(P1->x + slot->userOffsetX,
            P1->y + slot->userOffsetY);
        slot->SetChildPosition(P1->x + slot->userOffsetX,
            P1->y + slot->userOffsetY);
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
    if (action == 1) {
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
    lights.emplace_back(Vector2(480 + (bonfire->x - P1->x), 360.0 - (bonfire->y - P1->y)));
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
        if (auto env = dynamic_cast<Enemy*>(obj.get())) {
            env->moveTo(P1->x,P1->y, deltaTime);
            if (P1->CheckCollision(env) && onhit==false) {
                P1->onHit(count, env->x, env->y);
                SoundManager::GetInstance()->PlaySoundnoLoop("onhit");
                onhit = true;
                onhitdltime = 0;
            }
        }
        if (auto env = dynamic_cast<Boss*>(obj.get())) {
            env->moveTo(P1->x, P1->y, deltaTime);
            if (P1->CheckCollision(env) && onhit == false) {
                P1->onHit(count, env->x, env->y);
                SoundManager::GetInstance()->PlaySoundnoLoop("onhit");
                onhit = true;
                onhitdltime = 0;
            }
        }
        obj.get()->Update(deltaTime);
    }
    for (auto obj : i_bonfire) {
        obj->Update(deltaTime);
    }
    if (keyState['W'] ) {
        if (action == 0 ) {
            isWalk = true;
            hasCollision = false;
            count = 1;
            float newY = P1->y - movement_speed * deltaTime;
            temp = *P1;
            temp.y = newY;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision || onhit == true) {
                P1->MoveUp(newY, IsWaterTile(P1->x, newY));
            }
        }
    }
    if (keyState['D']) {
        if (action == 0 ) {
            isWalk = true;
            hasCollision = false;
            count = 2;
            float newX = P1->x + movement_speed * deltaTime;
            temp = *P1;
            temp.x = newX;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision || onhit == true) {
                P1->MoveRight(newX, IsWaterTile(newX, P1->y));
            }
        }
    }
    if (keyState['A'] ) {
        if (action == 0 ) {
            isWalk = true;
            hasCollision = false;
            count = 4;
            float newX = P1->x - movement_speed * deltaTime;
            temp = *P1;
            temp.x = newX;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision || onhit == true) {
                P1->MoveLeft(newX, IsWaterTile(newX, P1->y));
            }
        }
    }
    if (keyState['S'] ) {
        if (action == 0) {
            isWalk = true;
            hasCollision = false;
            count = 3;
            float newY = P1->y + movement_speed * deltaTime;
            temp.y = newY;

            for (size_t i = 0; i < envi_objects.size(); ++i) {
                if (temp.CheckCollision(envi_objects[i].get())) {
                    hasCollision = true;
                    break;
                }
            }
            if (!hasCollision || onhit == true) {
                P1->MoveDown(newY, IsWaterTile(P1->x, newY));
            }
        }
    }
    if (keyState['J'] )
    {   
        if (Slot::GetCurrentSlot()->HasItem()) {
            if (Slot::GetCurrentSlot()->GetItem()->m_category == "pickaxe") {
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
            else if (Slot::GetCurrentSlot()->GetItem()->m_category == "axe") {
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
                for (auto& org : enermy_objects) {
                    if (P1->GetHitbox(count)->CheckCollisionEnermy(org.get()))
                    {
                        if (auto env = dynamic_cast<Enemy*>(org.get())) {
                            env->onHit(count);
                        }
                    }
                }
                for (auto& org : Boss_objects) {
                    if (P1->GetHitbox(count)->CheckCollisionEnermy(org.get()))
                    {
                        if (auto env = dynamic_cast<Boss*>(org.get())) {
                            env->onHit(count);
                        }
                    }
                }
            }
            else if (Slot::GetCurrentSlot()->GetItem()->m_type == "food" && useItem == false) {
				printf("Energy: %f\n", Slot::GetCurrentSlot()->GetItem()->m_energy);
				P1->mp += Slot::GetCurrentSlot()->GetItem()->m_energy;
                PlayerInventory::GetInstance()->RemoveItemByIdHotbar(Slot::GetCurrentSlot()->GetItem()->m_id, 1);
				useItem = true;
				useitemdltime = 0.0f;
            }
        }
    }

    if (keyState['E'])
    {
        if (uidltime >= 0.2f) {
            uidltime = 0.0f;
            inventory->SetVisible(!inventory->IsVisible());
        }
    }
    if (keyState['Q'])
    {
        if (uidltime >= 0.2f) {
            uidltime = 0.0f;
			PlayerInventory::GetInstance()->PrintAllSlots();
        }
	}
    int previousSlot = currentSlot;

    if (keyState['1']) {
        Slot::SetCurrentSlot(0);
    }
    if (keyState['2']) {
        Slot::SetCurrentSlot(1);
	}
    if (keyState['3']) {
        Slot::SetCurrentSlot(2);
    }
    if (keyState['4']) {
        Slot::SetCurrentSlot(3);
    }
    if (keyState['5']) {
        Slot::SetCurrentSlot(4);
    }
    if (keyState['6']) {
        Slot::SetCurrentSlot(5);
    }
    if (keyState['7']) {
        Slot::SetCurrentSlot(6);
    }
    if (keyState['8']) {
        Slot::SetCurrentSlot(7);
    }
    if (keyState['9']) {
        Slot::SetCurrentSlot(8);
    }
    if (keyState['0']) {
        Slot::SetCurrentSlot(9);
	}
	
    // Nếu slot thay đổi thì cập nhật slot được chọn
    if (previousSlot != currentSlot) {
        Slot::SetCurrentSlot(currentSlot);
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
    for (auto& hb : hotbar) {
        hb->Draw();
    }
    for (auto& slot : m_craftingSlots) {
        slot->Draw();
    }
    button_play->Draw();
    hp->Draw();
    mp->Draw();
}

