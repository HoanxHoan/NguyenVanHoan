
#include "placeable_checker.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void placeable_checker::_ready()
{
	parent = this->get_parent();
	sprite = parent->get_node("WorkbenchTexture");
	collision_shape_2d = get_node("CollisionShape2D");
	original_color = sprite->modulate;
}

void placeable_checker::_process(double delta)
{

}

void placeable_checker::_on_body_entered(Ref<Node2D> body)
{
	UtilityFunctions::print("Bắt đầu va chạm với:", body->get_name());
	if(body->is_in_group("Buildings") |  | body->get_name() == "TileMapLayer")
	{
		sprite->modulate = Color(0.8, 0, 0, 0.5);
		available = false;
	}
	else
	{
		sprite->modulate = original_color;
		available = true;
	}
}

void placeable_checker::_on_body_exited(Ref<Node2D> body)
{
	sprite->modulate = original_color;
	available = true;
}

void placeable_checker::_bind_methods() {

}

