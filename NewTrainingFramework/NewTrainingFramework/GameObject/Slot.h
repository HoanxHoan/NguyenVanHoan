#pragma once
#include "GameButton.h"
#include "Item.h" // bạn cần có class Item
#include <random>
#include "../GameManager/PlayerInventory.h"

class PlayerInventory;

enum class SlotType {
    HOTBAR = 0,
    INVENTORY
};

class Slot : public GameButton {
public:
    Slot(Model* model, Texture* texture, Shaders* shader);
    ~Slot();
    void ClearHotbarSlots();
    static Slot* selectedSlot;
    static Slot* chosenSlot;
    static std::vector<Slot*> hotbarSlots;
    void RemoveItem();
    void SetSlotType(SlotType type);
    SlotType GetSlotType();
    void SetSlotIndex(int index);
    int GetSlotIndex();
    void SetItem(const std::shared_ptr<Item>& newItem);
    void SetItem(const std::shared_ptr<Item>& newItem, int newQuantity);
    void SetChildPosition(float x, float y);
    std::shared_ptr<Item> GetItem() const;
    bool HandleTouchEvents(GLint x, GLint y, bool bIsPressed);
    int GetQuantity();
    void SetQuantity(int newQuantity);
    void Draw();
    static void RegisterHotbarSlot(Slot* slot);
    static void SetCurrentSlot(int numpad);
    bool IsCurrentSlot();
    bool HasItem() const;
    PlayerInventory* ownerInventory = nullptr;

    PlayerInventory* GetOwnerInventory() { return ownerInventory; }
    void NotifyInventoryChanged();
    void SetOwnerInventory(PlayerInventory* inventory);
    std::shared_ptr<Item> item;
private:

    SlotType slotType;
    int slotIndex;
    Vector2 m_pos;
};