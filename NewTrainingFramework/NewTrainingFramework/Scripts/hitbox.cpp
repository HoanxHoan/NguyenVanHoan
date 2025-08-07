
#include "hitbox.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void hitbox::_ready()
{

}

void hitbox::get_weapon_id()
{
	return get_parent()->current_item_id;
}

void hitbox::get_weapon_damage()
{
	return get_parent()->current_attack_damage;
}

void hitbox::_process(double delta)
{

}

void hitbox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_weapon_id"), &hitbox::get_weapon_id);
	ClassDB::bind_method(D_METHOD("get_weapon_damage"), &hitbox::get_weapon_damage);

}

