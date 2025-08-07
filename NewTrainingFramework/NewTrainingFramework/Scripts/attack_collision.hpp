
#ifndef ATTACK_COLLISION_H
#define ATTACK_COLLISION_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

using namespace godot;

// Called when the node enters the scene tree for the first time.
class attack_collision : public CollisionShape2D {
	GDCLASS(attack_collision, CollisionShape2D);
public:

// Replace with function body.
	void _ready() override;

	void get_weapon_id();

// Called every frame. 'delta' is the elapsed time since the previous frame.
	void get_weapon_damage();

	void _process(double delta) override;

	static void _bind_methods();
};

#endif // ATTACK_COLLISION_H
