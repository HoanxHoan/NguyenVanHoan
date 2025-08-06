
#ifndef ITEM_H
#define ITEM_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

using namespace godot;

class item : public Control {
	GDCLASS(item, Control);
public:

protected:
	Ref<TextureRect> item_texture;
	Ref<Label> item_quantity;

// Called when the node enters the scene tree for the first time.
	String idName = "";
	String itemName = "";
	String itemType = "";
	String itemCategory = "";
	Variant item_amount;

	const String TOOLS_PATH = "res://Texture/Icons/Tools/";
	const String FOOD_PATH = "res://Texture/Icons/Food/";
	const String MATERIAL_PATH = "res://Texture/Icons/Materials/";
	const String PLACEABLE_PATH = "res://Texture/Environment/Structures/";

	Dictionary item_data = Dictionary();

// Called when the node enters the scene tree for the first time.

public:
	void _ready() override;

// Called every frame. 'delta' is the elapsed time since the previous frame.
	void load_item(String id);

	void _process(double delta) override;

	void add_item_quantity(Variant amount_to_add);

	void decrease_item_quantity(Variant amount_to_decrease);

	void set_item(Variant iN, Variant iA);
	void set_idName(String value);
	String get_idName();

	static void _bind_methods();
};

#endif // ITEM_H
