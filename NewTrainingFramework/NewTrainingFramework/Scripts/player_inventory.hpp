
#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class player_inventory : public Node {
	GDCLASS(player_inventory, Node);
public:
	/* signal active_item_updated(Variant slot_index) */
	/* signal remove_the_item(Variant slot_index) */
	/* signal inventory_updated() */

protected:
	const Ref<Resource> slotClass = /* preload has no equivalent, add a 'ResourcePreloader' Node in your scene */("res://Scripts/Inventory/inventory_slot.gd");
	const Ref<Resource> ItemClass = /* preload has no equivalent, add a 'ResourcePreloader' Node in your scene */("res://Scripts/Items/item.gd");

	Ref<Node> player;

	const int NUM_INVENTORY_SLOTS = 30;
	const int NUM_HOTBAR_SLOTS = 10;

	Dictionary inventory = Dictionary {/* initializer lists are unsupported */ 
		{0, Array {/* initializer lists are unsupported */ "wooden_axe", 1,  }},
		{15, Array {/* initializer lists are unsupported */ "wood_log", 15,  }},
		{16, Array {/* initializer lists are unsupported */ "wood_log", 12,  }},
		{20, Array {/* initializer lists are unsupported */ "stone", 14,  }},
		 };

	Dictionary hotbar = Dictionary {/* initializer lists are unsupported */ 

		{1, Array {/* initializer lists are unsupported */ "wooden_axe", 1,  }},
		{5, Array {/* initializer lists are unsupported */ "wooden_workbench", 1,  }},
		 };

	int active_item_slot = 0;

public:
	void add_item(Variant idName, Variant item_amount);

	void remove_item(Ref<slotClass> slot);

	void add_item_to_empty_slot(Ref<ItemClass> item, Ref<slotClass> slot);

	void add_item_quantity(Ref<slotClass> slot, int quantity_to_add);

// ✅ FIXED
	void active_item_scroll_up();

// ✅ FIXED
	void active_item_scroll_down();

	void set_active_slot(int index);

	void decrease_item(int num);

	Dictionary get_total_item_counts();

	void remove_item_by_id(String item_id, int amount);
	void _ready() override;

	static void _bind_methods();
};

#endif // PLAYER_INVENTORY_H
