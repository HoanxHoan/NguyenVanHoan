#pragma once
#include <string>
#include "Object.h"
#include "../GameManager/ResourceManager.h"

class Item
{
public:
    Item(const std::string& id = "", int amount = 1);
    ~Item();

    void Draw();

    void SetItem(const std::string& id, int amount);
    void LoadItem(const std::string& id);

    void AddQuantity(int amount);
    void DecreaseQuantity(int amount);

    Texture* GetTexture() const { return m_texture; }
    int GetAmount() const { return m_amount; }
    std::string GetId() const { return m_id; }

private:
    std::string m_id;
    int m_amount;

    std::string m_name;
    std::string m_type;
    std::string m_category;
    int m_iconId;

    Texture* m_texture;

    void LoadTexture(int icon_id);
};