#pragma once
#include "GameButton.h"
#include "Item.h" // bạn cần có class Item

enum class SlotType {
    HOTBAR = 0,
    INVENTORY
};

class Slot : public GameButton {
public:
    Slot(Model* model, Texture* texture, Shaders* shader);
    ~Slot();

    void PickFromSlot();
    void RemoveItem();
    void PutInSlot(std::shared_ptr<Item> newItem);
    void InitializeItem(const std::string& idName, int amount);
    void RefreshStyle();
    void SetSlotType(SlotType type);
    void SetSlotIndex(int index);
    std::shared_ptr<Item> GetItem() const;
    bool HandleTouchEvents(GLint x, GLint y, bool bIsPressed);
    void SetItem(const std::shared_ptr<Item>& newItem);
	
    bool HasItem() const;

private:
    std::shared_ptr<Item> item;
    SlotType slotType;
    int slotIndex;
};