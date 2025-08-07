
#ifndef WOODEN_WORKBENCH_H
#define WOODEN_WORKBENCH_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/static_body2d.hpp>

using namespace godot;

class wooden_workbench : public StaticBody2D {
	GDCLASS(wooden_workbench, StaticBody2D);
public:

protected:
	Ref<Node> player;
	Ref<CollisionShape2D> workbench_collision;
	Ref<CollisionShape2D> collision_shape_2d;
	Ref<Node> loot_base;
	Ref<AnimationPlayer> hit_flash;
	Ref<CollisionShape2D> crafting_area_collision;

	int hp = 5;
	bool is_placed = true;

	Ref<Resource> drop = /* preload has no equivalent, add a 'ResourcePreloader' Node in your scene */("res://Scenes/Drops/drop.tscn");

// Called when the node enters the scene tree for the first time.
// Called every frame. 'delta' is the elapsed time since the previous frame.

public:
	void _ready() override;

	void _process(double delta) override;

	void _physics_process(double delta) override;

	void death();

	void _on_hurt_box_area_entered(Ref<Area2D> area);

	void _on_crafting_area_body_entered(Ref<Node2D> body);

	void _on_crafting_area_body_exited(Ref<Node2D> body);

	static void _bind_methods();
};

#endif // WOODEN_WORKBENCH_H
