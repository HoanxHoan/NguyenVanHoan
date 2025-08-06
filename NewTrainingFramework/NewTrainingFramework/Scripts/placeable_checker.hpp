
#ifndef PLACEABLE_CHECKER_H
#define PLACEABLE_CHECKER_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>

using namespace godot;

class placeable_checker : public Area2D {
	GDCLASS(placeable_checker, Area2D);
public:

protected:
	Ref<Node> parent;
	Ref<Node> sprite;
	Ref<CollisionShape2D> collision_shape_2d;

	bool available = true;
	Variant original_color;

// Called when the node enters the scene tree for the first time.

// Called every frame. 'delta' is the elapsed time since the previous frame.

public:
	void _ready() override;

	void _process(double delta) override;

	void _on_body_entered(Ref<Node2D> body);

	void _on_body_exited(Ref<Node2D> body);

	static void _bind_methods();
};

#endif // PLACEABLE_CHECKER_H
