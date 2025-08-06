#include "InventorySlots.h"


void InventorySlots::AddSlot(std::shared_ptr<Slot> slot) {
    slots.push_back(slot);
	printf("added slot %d\n", slots.size() - 1);
}

const std::vector<std::shared_ptr<Slot>>& InventorySlots::GetSlots() {
    return slots;
}

void InventorySlots::Draw() {
    for (auto& slot : slots) {
        slot->Draw();
    }
}


void InventorySlots::Update(float x, float y, float deltaTime) {
    
    for (auto& slot : slots) {
        int cols = 10;
        int row = i / cols;
        int col = i % cols;
        float offsetX = -90 + col * 20.0f;
        float offsetY = 40 - row * 20.0f;
        slot->set2Dposition(x + offsetX, y + offsetY);
        i++;
        if (i >= 30) {
            i = 0;
        }
    }
}