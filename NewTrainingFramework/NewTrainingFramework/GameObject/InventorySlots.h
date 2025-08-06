#pragma once
#include <vector>
#include <memory>
#include "Slot.h"
#include "Object.h"

class InventorySlots : public Object {
public:
    InventorySlots(Model* model, Texture* texture, Shaders* shader)
		: Object(model, texture, shader) {
	}
    int i = 0;
    std::vector<std::shared_ptr<Slot>> slots;
    std::vector<std::shared_ptr<Slot>> m_slots;
    void AddSlot(std::shared_ptr<Slot> slot);
	void Update(float x, float y, float deltaTime) ;
    const std::vector<std::shared_ptr<Slot>>& GetSlots();
    void Draw();
private:
};