#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Slot.h"
#include "../GameManager/PlayerInventory.h"
#include "../GameManager/CraftingRecipeDB.h" // bạn cần định nghĩa tương tự ItemDB, chứa nguyên liệu và kết quả

class CraftingUI {
public:
    CraftingUI(std::shared_ptr<PlayerInventory> inventory);
    CraftingUI();
    void InitializeUI();
    void Draw();
    void HandleClickOnSlot(int slotIndex);
    void UpdateCraftableList();
    PlayerInventory* m_inventory;
    std::vector<std::shared_ptr<Slot>> m_slots; // danh sách slot crafting
    std::vector<std::string> m_craftableIds;    // danh sách id item có thể craft
    int m_slotsPerRow = 12;
    void RefreshUI();

    std::vector<std::string> GetCraftableItems();
    bool CanCraft(const std::string& recipeId);
    void CraftItem(const std::string& recipeId);
private:
   
};