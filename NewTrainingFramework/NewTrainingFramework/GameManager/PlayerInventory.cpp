#include "PlayerInventory.h"

PlayerInventory* PlayerInventory::instance = nullptr;

PlayerInventory::PlayerInventory() {

    updated = false;    
    reload = false;
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
    hotbar.clear();
	instance = nullptr;
}

void PlayerInventory::Destroy()
{
    delete instance;
    instance = nullptr;
}

void PlayerInventory::AddItem(const std::string& itemId, int amount) {
    int maxStack = ItemDB::GetInstance()->GetStackSize(itemId);

    // Cộng vào slot đã có sẵn item đó
    for (auto& pair : inventory) {
        auto& item = pair.second;
        if (item.first == itemId && amount > 0) {
            int space = maxStack - item.second;
            int addAmount = (space < amount) ? space : amount;
            item.second += addAmount;
            amount -= addAmount;
        }
    }

    // Tạo slot mới nếu còn dư
    for (int i = 0; i < NUM_INVENTORY_SLOTS && amount > 0; ++i) {
        if (inventory.count(i) == 0) {
            int addAmount = (maxStack < amount) ? maxStack : amount;
            inventory[i] = { itemId, addAmount };
            amount -= addAmount;
        }
    }

    // Nếu vẫn còn amount > 0 => inventory full
    if (amount > 0) {
        // Xử lý drop ra ngoài hoặc bỏ qua
        printf("Inventory full, %d %s dropped\n", amount, itemId.c_str());
    }
    reload = true;
    updated = true;
}


void PlayerInventory::RemoveItem(std::shared_ptr<Slot> slot) {
    if (slot->GetSlotType() == SlotType::HOTBAR) {
        hotbar.erase(slot->GetSlotIndex());
    }
    else if (slot->GetSlotType() == SlotType::INVENTORY) {
        inventory.erase(slot->GetSlotIndex());
    }
    else if (slot->GetSlotType() == SlotType::CRAFTING) {
        craftingbar.erase(slot->GetSlotIndex());
    }
    updated = true;
}

void PlayerInventory::AddItemToEmptySlot(Item* item, std::shared_ptr<Slot> slot) {
    int index = slot->GetSlotIndex();
    if (slot->GetSlotType() == SlotType::HOTBAR) {
        hotbar[index] = { item->GetIdName(), item->GetAmount() };
    }
    else if (slot->GetSlotType() == SlotType::INVENTORY) {
        inventory[index] = { item->GetIdName(), item->GetAmount() };
    }
    else if (slot->GetSlotType() == SlotType::CRAFTING) {
        craftingbar[index] = { item->GetIdName(), item->GetAmount() };
    }
    updated = true;
}

void PlayerInventory::AddItemQuantity(std::shared_ptr<Slot> slot, int amount) {
    if (slot->GetSlotType() == SlotType::HOTBAR) {
        hotbar[slot->GetSlotIndex()].second += amount;
    }
    else if (slot->GetSlotType() == SlotType::INVENTORY) {
        inventory[slot->GetSlotIndex()].second += amount;
    }
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
    // Duyệt qua tất cả slot inventory
    for (auto it = inventory.begin(); it != inventory.end() && amount > 0;) {
        auto& itemPair = it->second;
        if (itemPair.first == itemId) {
            if (itemPair.second > amount) {
                // Slot có đủ số lượng để trừ
                itemPair.second -= amount;

                // Cập nhật UI slot tương ứng
                for (auto& slot : Slot::allInventorySlots) {
                    if (slot->HasItem() && slot->GetItem()->GetIdName() == itemId) {
                        slot->GetItem()->DecreaseQuantity(amount);
                        break; // chỉ cập nhật 1 slot là đủ
                    }
                }

                amount = 0; // đã trừ xong
                updated = true;
                return;
            }
            else {
                // Slot không đủ, xóa hết slot này
                amount -= itemPair.second;
                it = inventory.erase(it); // xóa slot này và tiếp tục
                 // không increment iterator vì erase trả về iterator mới
                // Xóa UI slot
                for (auto& slot : Slot::allInventorySlots) {
                    if (slot->HasItem() && slot->GetItem()->GetIdName() == itemId) {
                        slot->RemoveItem(); // tự động UpdateFromSlot
                        break;
                    }
                }
                continue; // không increment iterator vì erase trả về iterator mới

            }
        }
        ++it; // sang slot kế tiếp
    }

    updated = true;
}

void PlayerInventory::RemoveItemByIdHotbar(const std::string& itemId, int amount) {
    // Duyệt qua tất cả slot inventory
    for (auto it = hotbar.begin(); it != hotbar.end() && amount > 0;) {
        auto& itemPair = it->second;
        if (itemPair.first == itemId) {
            if (itemPair.second > amount) {
                // Slot có đủ số lượng để trừ
                itemPair.second -= amount;

                // Cập nhật UI slot tương ứng
                for (auto& slot : Slot::hotbarSlots) {
                    if (slot->HasItem() && slot->GetItem()->GetIdName() == itemId) {
                        slot->GetItem()->DecreaseQuantity(amount);
                        break; // chỉ cập nhật 1 slot là đủ
                    }
                }

                amount = 0; // đã trừ xong
                updated = true;
                return;
            }
            else {
                // Slot không đủ, xóa hết slot này
                amount -= itemPair.second;
                it = hotbar.erase(it); // xóa slot này và tiếp tục
                 // không increment iterator vì erase trả về iterator mới
                // Xóa UI slot
                for (auto& slot : Slot::hotbarSlots) {
                    if (slot->HasItem() && slot->GetItem()->GetIdName() == itemId) {
                        slot->RemoveItem(); // tự động UpdateFromSlot
                        break;
                    }
                }
                continue; // không increment iterator vì erase trả về iterator mới

            }
        }
        ++it; // sang slot kế tiếp
    }

    updated = true;
}

void PlayerInventory::UpdateFromSlot(Slot* slot) {
    int index = slot->GetSlotIndex();
    if (index < 0) return;

    auto item = slot->GetItem();
    if (!item) { // slot trống -> xóa khỏi map nếu có
        if (slot->GetSlotType() == SlotType::HOTBAR)
            hotbar.erase(index);
        else if (slot->GetSlotType() == SlotType::INVENTORY)
            inventory.erase(index);
        else if (slot->GetSlotType() == SlotType::CRAFTING)
            craftingbar.erase(index);
        return;
    }

    // slot có item -> thêm hoặc cập nhật map
    if (slot->GetSlotType() == SlotType::HOTBAR)
        hotbar[index] = { item->GetIdName(), item->GetAmount() };
    else if (slot->GetSlotType() == SlotType::INVENTORY)
        inventory[index] = { item->GetIdName(), item->GetAmount() };
    else if (slot->GetSlotType() == SlotType::CRAFTING)
        craftingbar[index] = { item->GetIdName(), item->GetAmount() };

    updated = true;
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
    std::vector<std::shared_ptr<Slot>>& hotbarSlots, std::vector<std::shared_ptr<Slot>>& craftingSlots) {
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
    for (auto& pair : craftingbar) {
            int index = pair.first;
            const auto& itemId = pair.second.first;
            int amount = pair.second.second;
            if (index < craftingSlots.size()) {
                craftingSlots[index]->SetItem(std::make_shared<Item>(itemId, amount));
            }
    }

    updated = true;
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

void PlayerInventory::ClearAllItems() {
    inventory.clear();
    hotbar.clear();
    updated = true;
}

void PlayerInventory::PrintAllSlots() const {
    printf("=== Inventory Slots ===\n");
    for (const auto& pair : inventory) {
        int index = pair.first;
        const auto& itemId = pair.second.first;
        int amount = pair.second.second;
        printf("Slot %d: Item = %s, Amount = %d\n", index, itemId.c_str(), amount);
    }

    printf("=== Hotbar Slots ===\n");
    for (const auto& pair : hotbar) {
        int index = pair.first;
        const auto& itemId = pair.second.first;
        int amount = pair.second.second;
        printf("Hotbar %d: Item = %s, Amount = %d\n", index, itemId.c_str(), amount);
    }

    printf("=== Crafting Slots ===\n");
    for (const auto& pair : craftingbar) {
        int index = pair.first;
        const auto& itemId = pair.second.first;
        int amount = pair.second.second;
        printf("Crafting %d: Item = %s, Amount = %d\n", index, itemId.c_str(), amount);
    }

    printf("======================\n");
}

void PlayerInventory::Reset() {
    inventory.clear();
    hotbar.clear();
    craftingbar.clear();
    stationsNearby.clear();
    activeSlotIndex = 0;
    updated = true;
    reload = true;
}

void PlayerInventory::AddStartingItems() {
    hotbar[0] = { "wooden_pickaxe", 1 };
    hotbar[1] = { "wooden_axe", 1 };
    hotbar[2] = { "wooden_sword", 1 };
    hotbar[3] = { "cooked_beef", 5 };
	hotbar[4] = { "slime_summoner", 1 };
	hotbar[5] = { "golem_summoner", 1 };
    hotbar[6] = { "death_summoner", 1 };
	hotbar[7] = { "the_portal", 1 };
}