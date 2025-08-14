#include "CraftingUI.h"
#include <cstdio>

CraftingUI::CraftingUI(std::shared_ptr<PlayerInventory> inventory)
:m_inventory(inventory){
}
    

void CraftingUI::InitializeUI() {
    UpdateCraftableList();
    m_slots.clear();

    Model* m_slotModel = ResourceManager::GetInstance()->GetModel(2);
    Texture* m_slotTexture = ResourceManager::GetInstance()->GetTexture(70);
    Shaders* m_slotShader = ResourceManager::GetInstance()->GetShader(0);

    for (size_t i = 0; i < m_craftableIds.size(); ++i) {
        auto slot = std::make_shared<Slot>(m_slotModel, m_slotTexture, m_slotShader);

        // Gán loại slot và index
        slot->SetSlotType(SlotType::CRAFTING); // Hoặc SlotType::CRAFTING nếu bạn có enum này
        slot->SetSlotIndex(static_cast<int>(i));

        // Gán item đại diện cho công thức
        auto item = std::make_shared<Item>(m_craftableIds[i], 1);
        slot->SetItem(item);

        slot->setSize(13, 13);
        slot->SetSize(30, 30);
        // Tính vị trí theo hàng & cột
        int row = static_cast<int>(i) / m_slotsPerRow;
        int col = static_cast<int>(i) % m_slotsPerRow;
        slot->SetChildPosition(col * 64.0f, row * 64.0f);
        slot->set2Dposition(col * 64.0f, row * 64.0f);

        // Thêm vào danh sách slot
        m_slots.push_back(slot);
    }
}

void CraftingUI::UpdateCraftableList() {
    m_craftableIds = GetCraftableItems();

    if (m_craftableIds.empty()) {
        // Không còn món nào craft được, xóa hết slot UI
        m_slots.clear();
        return;  // Kết thúc hàm luôn, không tạo slot mới
    }

    // Nếu số lượng slot != số lượng recipe → tạo mới
    if (m_slots.size() != m_craftableIds.size()) {
        m_slots.clear();
        Model* m_slotModel = ResourceManager::GetInstance()->GetModel(2);
        Texture* m_slotTexture = ResourceManager::GetInstance()->GetTexture(70);
        Shaders* m_slotShader = ResourceManager::GetInstance()->GetShader(0);

        for (size_t i = 0; i < m_craftableIds.size(); ++i) {
            auto slot = std::make_shared<Slot>(m_slotModel, m_slotTexture, m_slotShader);
            slot->SetSlotType(SlotType::CRAFTING);
            slot->SetSlotIndex(static_cast<int>(i));
            slot->SetItem(std::make_shared<Item>(m_craftableIds[i], 1));

            int row = static_cast<int>(i) / m_slotsPerRow;
            int col = static_cast<int>(i) % m_slotsPerRow;
            slot->SetChildPosition(col * 64.0f, row * 64.0f);
            slot->set2Dposition(col * 64.0f, row * 64.0f);

            m_slots.push_back(slot);
        }
    }
    else {
        // Chỉ cập nhật item trong slot, không tạo lại
        for (size_t i = 0; i < m_craftableIds.size(); ++i) {
            m_slots[i]->SetItem(std::make_shared<Item>(m_craftableIds[i], 1));
        }
    }
}

    std::vector<std::string> CraftingUI::GetCraftableItems() {
    std::vector<std::string> result;
    auto totalItems = PlayerInventory::GetInstance()->GetTotalItemCounts();

    const auto& recipes = CraftingRecipeDB::GetInstance()->recipes;
    for (auto& kv : recipes) {
        const auto& recipeId = kv.first;
        const auto& recipeData = kv.second;
        bool canCraft = true;

        for (auto& ing : recipeData.ingredients) {
            if (totalItems[ing.item] < ing.amount) {
                int playerAmount = totalItems[ing.item];

                canCraft = false;
                break;
            }
        }

        if (!recipeData.station.empty()) {
            if (!PlayerInventory::GetInstance()->IsNearStation(recipeData.station)) {
                canCraft = false;
            }
        }

        if (canCraft) {
            for (auto& ing : recipeData.ingredients) {
            }
            result.push_back(recipeId);
        }
    }
    return result;
}

bool CraftingUI::CanCraft(const std::string& recipeId) {
    auto totalItems = PlayerInventory::GetInstance()->GetTotalItemCounts();
    const auto* recipe = CraftingRecipeDB::GetInstance()->GetRecipe(recipeId);
    if (!recipe) return false;
    for (auto& ing : recipe->ingredients) {
        if (totalItems[ing.item] < ing.amount) {

            printf("Checking ingredient: player has %d\n", totalItems[ing.item]);
            return false;
        }
    }
    return true;
}

void CraftingUI::CraftItem(const std::string& recipeId) {
    if (!CanCraft(recipeId)) {
        printf("Không đủ nguyên liệu để chế tạo %s\n", recipeId.c_str());
        return;
    }

    const auto* recipe = CraftingRecipeDB::GetInstance()->GetRecipe(recipeId);
    if (!recipe) return;

    // Trừ nguyên liệu
    for (auto& ing : recipe->ingredients) {
        PlayerInventory::GetInstance()->RemoveItemById(ing.item, ing.amount);
    }

    // Thêm thành phẩm — giả định tên công thức = tên item kết quả
    
    printf("Đã chế tạo: %s\n", recipeId.c_str());

}

void CraftingUI::HandleClickOnSlot(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= (int)m_slots.size()) return;
    auto item = m_slots[slotIndex]->GetItem();
    if (!item) return;

    CraftItem(item->GetIdName());
}

void CraftingUI::Draw() {
    for (auto& slot : m_slots) {
        slot->Draw();
    }
}

void CraftingUI::RefreshUI() {
    UpdateCraftableList(); // Chỉ cập nhật items, không clear layout
}