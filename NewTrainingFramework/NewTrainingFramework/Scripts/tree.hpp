
#ifndef TREE_H
#define TREE_H

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

class tree : public StaticBody2D {
	GDCLASS(tree, StaticBody2D);
public:

protected:
	Ref<AnimationPlayer> hit_flash;
	Ref<AnimationPlayer> tree_animation;
	Ref<Node> player;
	Ref<CollisionShape2D> collision;
	Ref<CollisionShape2D> collision_shape_2d;
	Ref<Node> loot_base;

	Ref<Resource> tree_drop = /* preload has no equivalent, add a 'ResourcePreloader' Node in your scene */("res://Scenes/Drops/drop.tscn");

	Vector2 playerpos = Vector2::ZERO;

	int hp = 10;

// Called when the node enters the scene tree for the first time.
// Called every frame. 'delta' is the elapsed time since the previous frame.

public:
	void _ready() override;

	void _process(double delta) override;

	void _on_hurt_box_body_entered(Ref<Node2D> body);

	void death();

	void _on_tree_animation_animation_finished(StringName anim_name);

	void _on_hurt_box_area_entered(Ref<Area2D> area);

	static void _bind_methods();
};

#endif // TREE_H
