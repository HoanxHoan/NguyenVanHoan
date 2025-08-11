#include "Item.h"

Item::Item(const std::string& id, int amount)
    : Object(nullptr, nullptr, nullptr), // tạm thời gọi ctor rỗng
    m_id(""), m_amount(amount), m_texture(nullptr)
{
    if (!id.empty()) {
        SetItem(id);
    }
}

Item::~Item()
{
    // KHÔNG delete m_texture nếu do ResourceManager quản lý
    m_texture = nullptr;
}

void Item::SetItem(const std::string& id)
{
    m_id = id;
    LoadItem(id);
	printf("Loaded item: %s\n", m_id.c_str());
    // Lấy các resource từ ResourceManager
    Model* model = ResourceManager::GetInstance()->GetModel(2);    // ví dụ model id = 0 (quad)
    Shaders* shader = ResourceManager::GetInstance()->GetShader(0); // shader dùng để hiển thị UI
    m_renderObject = std::make_shared<Object>(model, m_texture, shader);
}

void Item::LoadItem(const std::string& id)
{
    auto data = ItemDB::GetInstance()->GetItemData(id);
	printf("Loading item with ID: %s\n", id.c_str());
    if (data.empty()) {
        printf("Item ID '%s' does not exist in database\n", id.c_str());
        return;
    }

    m_name = data["display_name"];
    m_type = data["type"];
    m_category = data["category"];
    m_stackSize = std::stoi(data["stack_size"]);
    try {
        m_iconId = std::stoi(data["icon_id"]);
    }
    catch (const std::exception& e) {
        printf("Lỗi chuyển icon_id của item '%s': %s\n", id.c_str(), e.what());
        return;
    }
	printf("Item name: %s, type: %s, category: %s, icon_id: %d\n", m_name.c_str(), m_type.c_str(), m_category.c_str(), m_iconId);
    LoadTexture(m_iconId);

}

void Item::LoadTexture(int icon_id)
{
    m_texture = ResourceManager::GetInstance()->GetTexture(icon_id);
}

void Item::set2Dposition(float x, float y)
{
    if (m_renderObject) {
        m_renderObject->set2Dposition(200, 200);
    }
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

const std::string& Item::GetIdName() {
    return m_id;
}

int Item::GetStackSize()  {
    return m_stackSize;
}