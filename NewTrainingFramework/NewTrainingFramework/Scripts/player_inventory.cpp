
#include "player_inventory.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void player_inventory::add_item(Variant idName, Variant item_amount)
{
	for(Dictionary item : inventory)
	{
		if(inventory[item][0] == idName)
		{
			Variant stackSize = int(ItemDb.ITEMS[idName].stack_size);
			Variant able_to_add = stackSize - inventory[item][1];
			if(able_to_add >= item_amount)
			{
				inventory[item][1] += item_amount;
				return ;
			}
			else
			{
				inventory[item][1] += able_to_add;
				item_amount = item_amount - able_to_add;
			}
		}
	}
	for(int i=0; i<NUM_INVENTORY_SLOTS; i+=1)
	{
		if(inventory.has(i) == false)
		{
			inventory[i] = Array {/* initializer lists are unsupported */ idName, item_amount,  };
			return ;
		}
	}
}

void player_inventory::remove_item(Ref<slotClass> slot)
{
	if(slot->slot_type == slotClass->SlotType.HOTBAR)
	{
		hotbar.erase(slot->slot_index);
	}
	else
	{
		inventory.erase(slot->slot_index);
	}
}

void player_inventory::add_item_to_empty_slot(Ref<ItemClass> item, Ref<slotClass> slot)
{
	if(slot->slot_type == slotClass->SlotType.HOTBAR)
	{
		hotbar[slot->slot_index] = Array {/* initializer lists are unsupported */ item->idName, item->item_amount,  };
	}
	else
	{
		inventory[slot->slot_index] = Array {/* initializer lists are unsupported */ item->idName, item->item_amount,  };
	}
}

void player_inventory::add_item_quantity(Ref<slotClass> slot, int quantity_to_add)
{
	if(slot->slot_type == slotClass->SlotType.HOTBAR)
	{
		hotbar[slot->slot_index][1] += quantity_to_add;
	}
	else
	{
		inventory[slot->slot_index][1] += quantity_to_add;
	}
}

void player_inventory::active_item_scroll_up()
{
	active_item_slot = (active_item_slot + 1) % NUM_HOTBAR_SLOTS;
	player->update_current_item();
	emit_signal("active_item_updated", active_item_slot);
}

void player_inventory::active_item_scroll_down()
{
	if(active_item_slot == 0)
	{
		active_item_slot = NUM_HOTBAR_SLOTS - 1;
	}
	else
	{
		active_item_slot -= 1;
	}
	player->update_current_item();
	emit_signal("active_item_updated", active_item_slot);
}

void player_inventory::set_active_slot(int index)
{
	if(index != active_item_slot)
	{
		active_item_slot = index;
		player->update_current_item();
		emit_signal("active_item_updated", active_item_slot);
	}
}

void player_inventory::decrease_item(int num)
{
	int slot_index = active_item_slot;
	if(!hotbar.has(slot_index))
	{
		return ;

		// Slot trống, không làm gì

	}// Giảm số lượng
	hotbar[slot_index][1] -= num;

	// Nếu <= 0 thì xóa slot
	if(hotbar[slot_index][1] <= 0)
	{
		emit_signal("remove_the_item", active_item_slot);
		hotbar.erase(slot_index);
	}
}

Dictionary player_inventory::get_total_item_counts()
{
	Dictionary counts = Dictionary();
	for(Variant slot : inventory.values())
	{
		Variant item_id = slot[0];
		Variant amount = slot[1];
		if(item_id == null)
		{
			continue;
		}
		if(counts->has(item_id))
		{
			counts[item_id] += amount;
		}
		else
		{
			counts[item_id] = amount;
		}
	}
	UtilityFunctions::print(counts);
	return counts;
}

void player_inventory::remove_item_by_id(String item_id, int amount)
{

	// Duyệt qua inventory
	Array slots_to_remove = Array();
	for(Variant slot_index : inventory.keys())
	{
		if(inventory[slot_index][0] == item_id)
		{
			Variant current_amount = inventory[slot_index][1];
			if(current_amount > amount)
			{
				inventory[slot_index][1] -= amount;
				return ;
			}
			else
			{
				amount -= current_amount;
				slots_to_remove.append(slot_index);
				if(amount <= 0)
				{
					emit_signal("remove_the_item", slot_index);
					break;
				}
			}
		}
	}

	// Xóa slot nào đã hết
	for(Variant slot_index : slots_to_remove)
	{
		inventory.erase(slot_index);
	}
	UtilityFunctions::print("xong");
	emit_signal("inventory_updated");
}

void player_inventory::_ready()
{
	player = get_tree()->get_first_node_in_group("Player");
}

void player_inventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_item", "idName", "item_amount"), &player_inventory::add_item);
	ClassDB::bind_method(D_METHOD("remove_item", "slot"), &player_inventory::remove_item);
	ClassDB::bind_method(D_METHOD("add_item_to_empty_slot", "item", "slot"), &player_inventory::add_item_to_empty_slot);
	ClassDB::bind_method(D_METHOD("add_item_quantity", "slot", "quantity_to_add"), &player_inventory::add_item_quantity);
	ClassDB::bind_method(D_METHOD("active_item_scroll_up"), &player_inventory::active_item_scroll_up);
	ClassDB::bind_method(D_METHOD("active_item_scroll_down"), &player_inventory::active_item_scroll_down);
	ClassDB::bind_method(D_METHOD("set_active_slot", "index"), &player_inventory::set_active_slot);
	ClassDB::bind_method(D_METHOD("decrease_item", "num"), &player_inventory::decrease_item);
	ClassDB::bind_method(D_METHOD("get_total_item_counts"), &player_inventory::get_total_item_counts);
	ClassDB::bind_method(D_METHOD("remove_item_by_id", "item_id", "amount"), &player_inventory::remove_item_by_id);

	ClassDB::add_signal(get_class_static(), MethodInfo("active_item_updated", PropertyInfo(Variant::OBJECT, "slot_index")));
	ClassDB::add_signal(get_class_static(), MethodInfo("remove_the_item", PropertyInfo(Variant::OBJECT, "slot_index")));
	ClassDB::add_signal(get_class_static(), MethodInfo("inventory_updated"));
}

