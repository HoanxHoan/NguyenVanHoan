
#ifndef INVENTORY_SLOT_H
#define INVENTORY_SLOT_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class inventory_slot : public Panel {
	GDCLASS(inventory_slot, Panel);
public:

protected:
	Ref<Resource> ItemClass = /* preload has no equivalent, add a 'ResourcePreloader' Node in your scene */("res://Scenes/UI/Items/item.tscn");
	Variant item = null;
	Variant slot_index;
	Variant selected_texture;
	Variant slot_type;

public:
	enum SlotType {HOTBAR = 0, INVENTORY};

// Called when the node enters the scene tree for the first time.

	void _ready() override;

	void pickFromSlot();

	void remove_item();

	void putInSlot(Variant new_item);

	void update_item_position(Variant item);

	void initialize_item(Variant idName, Variant item_amount);

// Called every frame. 'delta' is the elapsed time since the previous frame.
	void refresh_style();

	void _process(double delta) override;

	static void _bind_methods();
};

VARIANT_ENUM_CAST(inventory_slot::SlotType)

#endif // INVENTORY_SLOT_H
