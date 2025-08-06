
#include "attack_collision.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void attack_collision::_ready()
{

}

void attack_collision::get_weapon_id()
{
	return get_parent()->current_weapon_id;
}

void attack_collision::get_weapon_damage()
{
	return get_parent()->current_attack_damage;
}

void attack_collision::_process(double delta)
{

}

void attack_collision::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_weapon_id"), &attack_collision::get_weapon_id);
	ClassDB::bind_method(D_METHOD("get_weapon_damage"), &attack_collision::get_weapon_damage);

}

