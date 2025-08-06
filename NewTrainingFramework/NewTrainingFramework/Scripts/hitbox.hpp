
#ifndef HITBOX_H
#define HITBOX_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/area2d.hpp>

using namespace godot;

// Called when the node enters the scene tree for the first time.
class hitbox : public Area2D {
	GDCLASS(hitbox, Area2D);
public:

// Replace with function body.
	void _ready() override;

	void get_weapon_id();

// Called every frame. 'delta' is the elapsed time since the previous frame.
	void get_weapon_damage();

	void _process(double delta) override;

	static void _bind_methods();
};

#endif // HITBOX_H
