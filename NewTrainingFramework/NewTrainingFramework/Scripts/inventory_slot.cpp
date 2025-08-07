
#include "inventory_slot.hpp"
#include "../GameObject/Object.h"

void inventory_slot::_ready()
{
	selected_texture = nullptr;
}

void inventory_slot::pickFromSlot()
{
	remove_child(item);
	Ref<Node> inventoryNode = find_parent("UserInterface");
	inventoryNode->add_child(item);
	item->mouse_filter = Control::MouseFilter::MOUSE_FILTER_IGNORE;
	call_deferred("update_item_position", item);
	item = null;
}

void inventory_slot::remove_item()
{
	remove_child(item);
	item = null;
}

void inventory_slot::putInSlot(Variant new_item)
{
	item = new_item;
	item->position = Vector2::ZERO;
	Ref<Node> inventoryNode = find_parent("UserInterface");
	if(item->get_parent() == inventoryNode)
	{
		inventoryNode->remove_child(item);
	}
	add_child(item);
	item->mouse_filter = Control::MouseFilter::MOUSE_FILTER_STOP;
}

void inventory_slot::update_item_position(Variant item)
{
	item->global_position = get_global_mouse_position();
}

void inventory_slot::initialize_item(Variant idName, Variant item_amount)
{
	if(item == null)
	{
		item = ItemClass->instantiate();
		add_child(item);
		item->set_item(idName, item_amount);
	}
	else
	{
		item->set_item(idName, item_amount);
	}
}

void inventory_slot::refresh_style()
{
	if(SlotType::HOTBAR == slot_type && PlayerInventory.active_item_slot == slot_index)
	{

	}
}

void inventory_slot::_process(double delta)
{

}

void inventory_slot::_bind_methods() {
	ClassDB::bind_method(D_METHOD("pickFromSlot"), &inventory_slot::pickFromSlot);
	ClassDB::bind_method(D_METHOD("remove_item"), &inventory_slot::remove_item);
	ClassDB::bind_method(D_METHOD("putInSlot", "new_item"), &inventory_slot::putInSlot);
	ClassDB::bind_method(D_METHOD("update_item_position", "item"), &inventory_slot::update_item_position);
	ClassDB::bind_method(D_METHOD("initialize_item", "idName", "item_amount"), &inventory_slot::initialize_item);
	ClassDB::bind_method(D_METHOD("refresh_style"), &inventory_slot::refresh_style);
	ClassDB::bind_integer_constant(get_class_static(), _gde_constant_get_enum_name(HOTBAR, "HOTBAR"), "HOTBAR", HOTBAR);
	ClassDB::bind_integer_constant(get_class_static(), _gde_constant_get_enum_name(INVENTORY, "INVENTORY"), "INVENTORY", INVENTORY);
}

