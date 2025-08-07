#include "Slot.h"

Slot::Slot(Model* model, Texture* texture, Shaders* shader)
    : GameButton(model, texture, shader), item(nullptr), slotType(SlotType::INVENTORY), slotIndex(-1)
{
}

Slot::~Slot() {}

void Slot::RemoveItem() {
    item = nullptr;
}

void Slot::SetItem(const std::shared_ptr<Item>& newItem) {
    item = newItem;
}

std::shared_ptr<Item> Slot::GetItem() const {
    return item;
}

bool Slot::HasItem() const {
    return item != nullptr;
}

void Slot::SetSlotType(SlotType type) {
    slotType = type;
}

void Slot::SetSlotIndex(int index) {
    slotIndex = index;
}



bool Slot::HandleTouchEvents(GLint x, GLint y, bool bIsPressed)
{
    bool handled = GameButton::HandleTouchEvents(x, y, bIsPressed);

    if (handled && !bIsPressed) {
        printf("Slot %d clicked (has item: %d)\n", slotIndex, HasItem());
    }

    return handled;
}