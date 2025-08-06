
#ifndef DROP_H
#define DROP_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

using namespace godot;

class drop : public Area2D {
	GDCLASS(drop, Area2D);
public:

protected:
	Ref<Sprite2D> drop_texture;
	Ref<CollisionShape2D> collision_shape_2d;

	String idName = "";
	String dropName = "";
	String dropType = "";
	String dropCategory = "";

	const String TOOLS_PATH = "res://Texture/Icons/Tools/";
	const String FOOD_PATH = "res://Texture/Icons/Food/";
	const String MATERIAL_PATH = "res://Texture/Icons/Materials/";
	const String PLACEABLE_PATH = "res://Texture/Environment/Structures/";

	Variant target = null;
	int speed = 0;

	Dictionary item_data = Dictionary();

// Called when the node enters the scene tree for the first time.

public:
	void _ready() override;

	void load_item(String id);

	void _physics_process(double delta) override;

	void collect();

	void set_target(Variant player, Variant delta);
	void set_idName(String value);
	String get_idName();

	static void _bind_methods();
};

#endif // DROP_H
