#pragma once
#include "GameButton.h"

#include "Item.h" // bạn cần có class Item
#include <random>
#include "../GameManager/PlayerInventory.h"

class PlayerInventory;


enum class SlotType {
    HOTBAR = 0,
    INVENTORY,
    CRAFTING
};

class Slot : public GameButton {
public:
    Slot(Model* model, Texture* texture, Shaders* shader);
    ~Slot();
    static void ClearHotbarSlots();
    static Slot* selectedSlot;
    static Slot* chosenSlot;
    static std::vector<Slot*> hotbarSlots;
    void RemoveItem();
    void SetSlotType(SlotType type);
    SlotType GetSlotType();
    void SetSlotIndex(int index);
    int GetSlotIndex();
    void SetItem(const std::shared_ptr<Item>& newItem);
    void SetChildPosition(float x, float y);
    void TextPosition(float x, float y);
    std::shared_ptr<Item> GetItem() const;
    bool HandleTouchEvents(GLint x, GLint y, bool bIsPressed);
    void Draw();
    static void RegisterHotbarSlot(Slot* slot);
    void RegisterCraftingSlot(Slot* slot);
    static void SetCurrentSlot(int numpad);
    bool IsCurrentSlot();
    bool HasItem() const;
    PlayerInventory* ownerInventory = nullptr;
	PlayerInventory* ownerCraftingInventory = nullptr;
    int userOffsetX = 0;
    int userOffsetY = 0;
    PlayerInventory* GetOwnerInventory() { return ownerInventory; }
    void NotifyInventoryChanged();
    void SetOwnerInventory(PlayerInventory* inventory);
    void SetOwnerCrafting(PlayerInventory* inventory);
    std::shared_ptr<Item> item;
    std::string Slot::GetLastItemId() const {
        return lastItemId;
    }
    Vector3 t_pos;
    std::shared_ptr<TextRenderer> textRenderer;
private:
    std::string lastItemId;
    SlotType slotType;
    int slotIndex;
    Vector2 m_pos;
};