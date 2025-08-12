#pragma once

#include "GameStateBase.h"
#include "Scene.h"
#include "GameStateMachine.h"
#include "GSMenu.h"
#include "GSPause.h"
#include "../GameObject/SpriteAnimation.h"
#include "../GameObject/Building.h"
#include "../GameObject/Tree.h"
#include "../GameObject/Stone.h"
#include "../GameObject/Bush.h"
#include "../GameObject/Animal.h"
#include "../GameObject/Enemy.h"
#include "../GameObject/Boss.h"
#include "../GameObject/Overlay.h"
#include "../GameObject/Player.h"
#include "../tinyxml2.h"
#include "../GameObject/Slot.h"
#include "../GameObject/CraftingUI.h"
#include <unordered_set>   // Thêm ở đầu file
#include <unordered_map>   // Nếu dùng unordered_map cũng cần thêm

// Và nếu chưa dùng namespace std đầy đủ, thêm:
using namespace std;
class GSPlay : public GameStateBase
{
public:
    GSPlay();
    ~GSPlay();

    bool Init() override;
    void UpdateCraftingSlots();
    void Exit() override;

    void Pause() override;
    void Resume() override;

    void Update(float deltaTime) override;
    void Draw() override;
    void HandleInput(unsigned char key, bool isPressed) override;
    void HandleMouseClick(GLint x, GLint y, bool isClick) override;
    void ReloadCraftingSlots();
    std::vector<std::shared_ptr<Slot>> GetAllSlots();
    StateType GetStateType() const override { return StateType::PLAY; }
    bool IsWaterTile(int xPixel, int yPixel);
    Vector3 GetRandomValidPosition();
    Vector3 GenerateRandomValidPositionAvoidCollision(const std::vector<std::shared_ptr<Object>>& others);
    tinyxml2::XMLDocument doc;
    std::shared_ptr<GameButton> button_play;
    std::shared_ptr<GameButton> button_play2;
    //std::shared_ptr<SpriteAnimation> P1;
    std::shared_ptr<Building>bonfire;
    std::shared_ptr<Building>fire;
    std::vector<std::shared_ptr<Slot>> inventorySlots;
    std::vector<std::shared_ptr<Slot>> hotbar;
    std::vector<std::shared_ptr<Slot>> hotbarSlots;
	std::shared_ptr<Slot> slot;
    std::shared_ptr<Slot> hotbar_slot;
    std::shared_ptr<Tree>tree;
    std::shared_ptr<Stone>stone;
    std::shared_ptr<Bush>bush;
    std::shared_ptr<Animal>animal;
    std::shared_ptr<Enemy>org;
    std::shared_ptr<Boss>boss;
    std::shared_ptr<Player>P1;
    std::shared_ptr<Overlay> overlay;
    std::vector<Vector2> lights;
	std::shared_ptr<Building> inventory;
    bool keyState[256] = { false };
    int x, y, count,action;
    float dltime = 0.0f, actiontime = 0.0f, uidltime = 0.0f;
    std::vector<std::shared_ptr<Object>> i_objects;
    //std::vector<Object*> i_objects;
    std::vector<std::shared_ptr<Object>> envi_objects;
    std::vector<std::shared_ptr<Object>> enermy_objects;
    std::vector<std::shared_ptr<Object>> Boss_objects;
    std::vector<Object*> i_bonfire;
    std::vector<int> waterTiles;
    int mapWidth ;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    int i = 0;
    int j = 0;
    int mapPixelWidth ;
    int mapPixelHeight ;
    int currentSlot = 0;
    int movement_speed = 30;
    Object temp;
    bool hasCollision = false;
    bool enermyCollision = false;
    std::shared_ptr<PlayerInventory> playerInventory;
    bool onhit;
    float onhitdltime;
    static std::shared_ptr<CraftingUI> m_craftingUI;
    static std::string m_recipeId;
    std::vector<std::shared_ptr<Slot>> craftingSlots;
    std::vector<std::shared_ptr<Slot>> m_craftingSlots;
    bool reloadable = false;
    bool isWalk = false;
    bool wasWalking = false;
};

