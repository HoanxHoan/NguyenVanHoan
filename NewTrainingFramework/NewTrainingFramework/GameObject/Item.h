#pragma once
#include <string>
#include "Object.h"
#include "../GameManager/ItemDb.h"
#include "../GameManager/ResourceManager.h"
#include "../GameObject/TextRenderer.h"

class Item : public Object {
public:
    Item(const std::string& id = "", int amount = 1);
    ~Item();
    std::shared_ptr<ItemDB> itemreader;
	//std::shared_ptr<TextRenderer> textRenderer;
    void SetItem(const std::string& id);
    void LoadItem(const std::string& id);

    void AddQuantity(int amount);
    void DecreaseQuantity(int amount);
    const std::string& GetIdName();
    int GetStackSize();
    void set2Dposition(float x, float y);
    int GetAmount() const { return m_amount; }
    std::string GetId() const { return m_id; }

    Texture* m_texture = nullptr;
    std::shared_ptr<Object>m_renderObject;
    std::string m_id;
    int m_amount;
    int m_stackSize;
    int m_energy;
    std::string m_name;
    std::string m_type;
    std::string m_category;
    int m_iconId = -1;
private:
    void LoadTexture(int icon_id);
};