#include "Slot.h"



Slot::Slot(Model* model, Texture* texture, Shaders* shader)
    : GameButton(model, texture, shader), item(nullptr), slotType(SlotType::INVENTORY), slotIndex(-1)
{
    // Khởi tạo random seed 1 lần duy nhất (đặt ở nơi khác nếu cần)
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(0)));
        seeded = true;
    }
    textRenderer = std::make_shared<TextRenderer>();
    textRenderer->Init("../Resources/Fonts/arial.ttf", 20);
    // 50% cơ hội tạo item
}

Slot* Slot::selectedSlot = nullptr;
Slot* Slot::chosenSlot = nullptr;
std::vector<Slot*> Slot::hotbarSlots;

Slot::~Slot() {
    printf("cleared\n");
}

void Slot::ClearHotbarSlots() {
    hotbarSlots.clear();
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

SlotType Slot::GetSlotType() {
    return slotType;
}

void Slot::SetSlotIndex(int index) {
    slotIndex = index;
}

int Slot::GetSlotIndex() {
    return slotIndex;
}


void Slot::SetItem(const std::shared_ptr<Item>& newItem) {
    if (newItem) {
        lastItemId = newItem->GetIdName();
    }
    else {
        // item bị clear, nhưng giữ lastItemId để nhớ item trước đó
    }
    item = newItem;
    NotifyInventoryChanged();
}

void Slot::RemoveItem() {
    item = nullptr;
    NotifyInventoryChanged();
}

void Slot::NotifyInventoryChanged() {

    PlayerInventory::GetInstance()->UpdateFromSlot(this);

}

void Slot::SetOwnerInventory(PlayerInventory* inventory) {
    ownerInventory = inventory;
}

void Slot::SetOwnerCrafting(PlayerInventory* inventory) {
    ownerCraftingInventory = inventory;
}

void Slot::SetChildPosition(float x, float y)
{
    m_pos.x = x;
    m_pos.y = y;

}

void Slot::TextPosition(float x, float y)
{
    t_pos.x = x + 10;
    t_pos.y = (720 - y) - 13;
}

void Slot::Draw()
{
    // Vẽ nền của slot
    bool isSelected = (selectedSlot == this);

    float drawSize = isSelected ? 15.0f : 8.0f;
    float offset = (15.0f - drawSize) / 2.0f;
    GameButton::Draw();

    if (item && item->m_renderObject) {
        item->m_renderObject->set2Dposition(m_pos.x, m_pos.y);
        item->m_renderObject->setSize(drawSize, drawSize); // Kích thước của item   
        item->m_renderObject->Draw();
        if(item->m_amount > 1){
            textRenderer->RenderText(std::to_string(item->m_amount), t_pos.x, t_pos.y, 0.5f, Vector3(1.0f, 1.0f, 1.0f));
        }
        else textRenderer->RenderText("", t_pos.x, t_pos.y, 0.5f, Vector3(1.0f, 1.0f, 1.0f));
    }
}

void Slot::RegisterHotbarSlot(Slot* slot) {
    hotbarSlots.push_back(slot);
}

void Slot::SetCurrentSlot(int index)
{
    if (index >= 0 && index < static_cast<int>(hotbarSlots.size())) {
        Slot* targetSlot = hotbarSlots[index];

        if (targetSlot) {
            chosenSlot = targetSlot;
            printf("Hotbar Slot %d selected via key\n", index);

            if (!targetSlot->HasItem()) {
                printf(" (but it's empty)\n");
            }
        }
    }
}

bool Slot::IsCurrentSlot() {
    return this == chosenSlot;
}

bool Slot::HandleTouchEvents(GLint x, GLint y, bool bIsPressed)
{
    bool handled = GameButton::HandleTouchEvents(x, y, bIsPressed);

    if (handled && !bIsPressed) {

        if (selectedSlot == nullptr) {
            // Lần đầu click: chọn slot này
            if (HasItem()) {
                selectedSlot = this;
                printf("Slot %d selected\n", slotIndex);
            }
        }
        else {
            if (selectedSlot == this) {
                // Click lại cùng slot => bỏ chọn
                selectedSlot = nullptr;
                printf("Deselected Slot %d\n", slotIndex);
            }
            else {
                // Click vào slot khác => hoán đổi hoặc di chuyển item
                if (!HasItem()) {
                    // Slot này trống -> chuyển item từ selectedSlot sang
                    SetItem(selectedSlot->GetItem());
                    selectedSlot->RemoveItem();
                }
                else {
                    // Cả 2 đều có item => hoán đổi
                    std::shared_ptr<Item> temp = item;

                    SetItem(selectedSlot->GetItem());
                    selectedSlot->SetItem(temp);
                }

                printf("Slot %d and %d swapped/moved\n", selectedSlot->slotIndex, slotIndex);
                selectedSlot = nullptr; // reset chọn
            }
        }
    }

    return handled;
}
