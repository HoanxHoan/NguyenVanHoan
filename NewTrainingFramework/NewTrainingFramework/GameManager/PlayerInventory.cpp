#include "PlayerInventory.h"

PlayerInventory* PlayerInventory::instance = nullptr;

PlayerInventory::PlayerInventory() {
    inventory[0] = { "wooden_log", 12 };
    inventory[3] = { "stone", 12 };

    hotbar[1] = { "wooden_pickaxe", 1};
}

PlayerInventory* PlayerInventory::GetInstance() {
    static PlayerInventory instance;
    return &instance;
}

PlayerInventory::~PlayerInventory() {
    // Destructor logic if needed
    for(auto& m : inventory) {
        printf("Item ID: %s, Amount: %d\n", m.second.first.c_str(), m.second.second);
	}
    for(auto& m : hotbar) {
        printf("Hotbar Item ID: %s, Amount: %d\n", m.second.first.c_str(), m.second.second);
    }
    inventory.clear();
    hotbar.clear();
	instance = nullptr;
}

void PlayerInventory::Destroy()
{
    delete instance;
    instance = nullptr;
}

void PlayerInventory::AddItem(const std::string& itemId, int amount) {
    for (auto& pair : inventory) {
        int index = pair.first;
        auto& item = pair.second;
        if (item.first == itemId) {
            int maxStack = ItemDB::GetInstance()->GetStackSize(itemId);
            int space = maxStack - item.second;
            if (space >= amount) {
                item.second += amount;
                return;
            }
            else {
                item.second += space;
                amount -= space;
            }
        }
    }

    for (int i = 0; i < NUM_INVENTORY_SLOTS; ++i) {
        if (inventory.count(i) == 0) {
            inventory[i] = { itemId, amount };
            return;
        }
    }
}


void PlayerInventory::RemoveItem(std::shared_ptr<Slot> slot) {
    if (slot->GetSlotType() == SlotType::HOTBAR) {
        hotbar.erase(slot->GetSlotIndex());
    }
    else {
        inventory.erase(slot->GetSlotIndex());
    }
}

void PlayerInventory::AddItemToEmptySlot(Item* item, std::shared_ptr<Slot> slot) {
    int index = slot->GetSlotIndex();
    if (slot->GetSlotType() == SlotType::HOTBAR) {
        hotbar[index] = { item->GetIdName(), item->GetAmount() };
    }
    else {
        inventory[index] = { item->GetIdName(), item->GetAmount() };
    }
}

void PlayerInventory::AddItemQuantity(std::shared_ptr<Slot> slot, int amount) {
    auto& map = slot->GetSlotType() == SlotType::HOTBAR ? hotbar : inventory;
    map[slot->GetSlotIndex()].second += amount;
}

void PlayerInventory::DecreaseActiveItem(int amount) {
    if (hotbar.count(activeSlotIndex) == 0) return;

    hotbar[activeSlotIndex].second -= amount;
    if (hotbar[activeSlotIndex].second <= 0) {
        hotbar.erase(activeSlotIndex);
        // Optional: Emit signal or notify game
    }
}

void PlayerInventory::SetActiveSlot(int index) {
    if (index != activeSlotIndex) {
        activeSlotIndex = index;
        // Notify update
    }
}

std::unordered_map<std::string, int> PlayerInventory::GetTotalItemCounts() {
    std::unordered_map<std::string, int> counts;
    for (const auto& pair : inventory) {
        const auto& item = pair.second;
        counts[item.first] += item.second;
    }
    return counts;
}

void PlayerInventory::RemoveItemById(const std::string& itemId, int amount) {
    std::vector<int> toRemove;
    for (auto& pair : inventory) {
        int index = pair.first;
        auto& item = pair.second;
        if (item.first == itemId) {
            if (item.second > amount) {
                item.second -= amount;
                return;
            }
            else {
                amount -= item.second;
                toRemove.push_back(index);
                if (amount <= 0) break;
            }
        }
    }

    for (int index : toRemove) {
        inventory.erase(index);
        // Optional: signal
    }
}

void PlayerInventory::UpdateFromSlot(Slot* slot) {
    int index = slot->GetSlotIndex();
    if (index < 0) return;

    auto item = slot->GetItem();

    if (slot->GetSlotType() == SlotType::HOTBAR) {
        if (index >= NUM_HOTBAR_SLOTS) return;
        if (item)
            hotbar[index] = { item->GetIdName(), item->GetAmount() };
        else
            hotbar.erase(index);
    }
    else {
        if (index >= NUM_INVENTORY_SLOTS) return;

        if (item)
        {
            inventory[index] = { item->GetIdName(), item->GetAmount() };
        }
        else
            inventory.erase(index);
    }
}

int ItemDB::GetStackSize(const std::string& id) {
    auto it = items.find(id);
    if (it == items.end()) return 1; // default nếu không tìm thấy

    auto& data = it->second;
    auto itStack = data.find("stack_size");
    if (itStack == data.end()) return 1;

    try {
        return std::stoi(itStack->second);
    }
    catch (...) {
        return 1;
    }
}


void PlayerInventory::InitializeUI(std::vector<std::shared_ptr<Slot>>& inventorySlots,
    std::vector<std::shared_ptr<Slot>>& hotbarSlots) {
    for (auto& pair : inventory) {
        int index = pair.first;
        const auto& itemId = pair.second.first;
        int amount = pair.second.second;
        if (index < inventorySlots.size()) {
            inventorySlots[index]->SetItem(std::make_shared<Item>(itemId, amount));
        }
    }

    for (auto& pair : hotbar) {
        int index = pair.first;
        const auto& itemId = pair.second.first;
        int amount = pair.second.second;
        if (index < hotbarSlots.size()) {
            hotbarSlots[index]->SetItem(std::make_shared<Item>(itemId, amount));
        }
    }
}

bool PlayerInventory::IsNearStation(const std::string& stationId) const {
    for (const auto& station : stationsNearby) {
        if (station == stationId) {
            return true;
        }
    }
    return false;
}

void PlayerInventory::AddNearbyStation(const std::string& stationId) {
    // Không thêm trùng
    for (const auto& station : stationsNearby) {
        if (station == stationId) {
            return;
        }
    }
    stationsNearby.push_back(stationId);
}

void PlayerInventory::RemoveNearbyStation(const std::string& stationId) {
    stationsNearby.erase(
        std::remove(stationsNearby.begin(), stationsNearby.end(), stationId),
        stationsNearby.end()
    );
}

void PlayerInventory::ClearNearbyStations() {
    stationsNearby.clear();
    stationsNearby.push_back(""); // giữ lại phần tử mặc định
}