#include "Item.h"
#include "../GameManager/ItemDb.h"  

Item::Item(const std::string& id, int amount)
    : m_id(""), m_amount(amount), m_texture(nullptr)
{
    if (!id.empty()) {
        SetItem(id, amount);
    }
}

Item::~Item()
{
    if (m_texture) {
        delete m_texture;
        m_texture = nullptr;
    }
}

void Item::SetItem(const std::string& id, int amount)
{
    m_id = id;
    m_amount = amount;
    LoadItem(id);
}

void Item::LoadItem(const std::string& id)
{
    auto data = ItemDB::GetItemData(id);
    if (data.empty()) {
        printf("Item ID '%s' không tồn tại trong database\n", id.c_str());
        return;
    }

    m_name = data["display_name"];
    m_type = data["type"];
    m_category = data["category"];

    try {
        m_iconId = std::stoi(data["icon_id"]);
    }
    catch (const std::exception& e) {
        printf("Lỗi chuyển icon_id của item '%s': %s\n", id.c_str(), e.what());
        return;
    }

    LoadTexture(m_iconId);
}

void Item::LoadTexture(int icon_id)
{
    if (m_texture) {
        delete m_texture;
        m_texture = nullptr;
    }

    m_texture = ResourceManager::GetInstance()->GetTexture(icon_id);
}

void Item::Draw()
{
    
}

void Item::AddQuantity(int amount)
{
    m_amount += amount;
}

void Item::DecreaseQuantity(int amount)
{
    m_amount -= amount;
    if (m_amount < 0) m_amount = 0;
}
