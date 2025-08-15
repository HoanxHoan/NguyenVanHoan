#pragma once
#include <string>
#include <memory>
#include "../GameObject/Item.h"
#include "../GameObject/Slot.h"

class Slot;

class PlayerInventory {
public:
    static PlayerInventory* instance;
    PlayerInventory();
    static PlayerInventory* GetInstance();
    ~PlayerInventory();
	static void Destroy();
    void AddItem(const std::string& itemId, int amount);
    void AddCraftItem(const std::string& itemId, int amount);
    void RemoveItem(std::shared_ptr<Slot> slot);
    void AddItemToEmptySlot(Item* item, std::shared_ptr<Slot> slot);
    void AddItemQuantity(std::shared_ptr<Slot> slot, int amount);
    void DecreaseActiveItem(int amount);
    bool reload = false;
    void SetActiveSlot(int index);
    std::unordered_map<std::string, int> GetTotalItemCounts();
    void RemoveItemById(const std::string& itemId, int amount);
    void Reset();
    void AddStartingItems();
    void RemoveItemByIdHotbar(const std::string& itemId, int amount);

    void UpdateFromSlot(Slot* slot);

    void InitializeUI(std::vector<std::shared_ptr<Slot>>& inventorySlots, std::vector<std::shared_ptr<Slot>>& hotbarSlots, std::vector<std::shared_ptr<Slot>>& craftingSlots);
    std::vector<std::string> stationsNearby = { "" };

    const std::unordered_map<int, std::pair<std::string, int>>& GetInventory() const { return inventory; }
    const std::unordered_map<int, std::pair<std::string, int>>& GetHotbar() const { return hotbar; }
    std::shared_ptr<Item> item;
    int GetActiveSlotIndex() const { return activeSlotIndex; }
    std::unordered_map<int, std::pair<std::string, int>> inventory;
    std::unordered_map<int, std::pair<std::string, int>> craftingbar;
    std::unordered_map<int, std::pair<std::string, int>> hotbar;
    void SetSelectedSlot(std::shared_ptr<Slot> slot);
    std::shared_ptr<Slot> GetSelectedSlot();
    void SetChosenSlot(std::shared_ptr<Slot> slot);
    std::shared_ptr<Slot> GetChosenSlot();
    void RegisterHotbarSlot(std::shared_ptr<Slot> slot);
    std::shared_ptr<Slot> GetHotbarSlot(int index);
    std::shared_ptr<Slot> selectedSlot;
    std::shared_ptr<Slot> chosenSlot ;
    std::vector<std::shared_ptr<Slot>> hotbarSlots;
    bool IsNearStation(const std::string& stationId) const;
    void AddNearbyStation(const std::string& stationId);
    void RemoveNearbyStation(const std::string& stationId);
    void ClearNearbyStations();
    void ClearAllItems();
    void PrintAllSlots() const;
    bool updated;
private:
    static constexpr int NUM_INVENTORY_SLOTS = 30;
    static constexpr int NUM_HOTBAR_SLOTS = 10;

    int activeSlotIndex = 0;
   
};