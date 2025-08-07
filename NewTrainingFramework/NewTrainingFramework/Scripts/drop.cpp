
#include "drop.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void drop::_ready()
{
	drop_texture = get_node("DropTexture");
	collision_shape_2d = get_node("CollisionShape2D");
	load_item(idName);
}

void drop::load_item(String id)
{
	if(!ItemDb.ITEMS.has(id))
	{
		UtilityFunctions::push_error("Item ID '%s' không tồn tại trong database" % id);
		return ;
	}

	item_data = ItemDb.ITEMS[id];

	Variant icon_name = item_data.get("icon_name", "");
	if(icon_name == "")
	{
		UtilityFunctions::push_error("Item '%s' không có 'icon_name'" % id);
		return ;
	}

	String texture_path = "";
	if(item_data.type == "material")
	{
		texture_path = MATERIAL_PATH;
	}
	else if(item_data.type == "food")
	{
		texture_path = FOOD_PATH;
	}
	else if(item_data.type == "tools")
	{
		texture_path = TOOLS_PATH;
	}
	else if(item_data.type == "placeable")
	{
		Variant dir_name = item_data.get("display_name", "");
		if(item_data.catagory == "station")
		{
			texture_path = PLACEABLE_PATH + "Stations/" + dir_name + "/";
		}
		else
		{
			texture_path = PLACEABLE_PATH + "Buildings/" + dir_name + "/";
		}
	}
	drop_texture->set_texture(load(texture_path + icon_name + ".png"));
}

void drop::_physics_process(double delta)
{
	if(target != null)
	{
		global_position = global_position.move_toward(target->global_position, speed);
		speed += 2 * delta;
	}
}

void drop::collect()
{
	collision_shape_2d->call_deferred("set", "disabled", true);
	drop_texture->set_visible(false);
	PlayerInventory.add_item(idName, 1);
	queue_free();
}

void drop::set_target(Variant player, Variant delta)
{
	target = player;
	speed += 5 * delta;
}

void drop::set_idName(String value) {
	idName = value;
}

String drop::get_idName() {
	return idName;
}

void drop::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_item", "id"), &drop::load_item);
	ClassDB::bind_method(D_METHOD("collect"), &drop::collect);
	ClassDB::bind_method(D_METHOD("set_target", "player", "delta"), &drop::set_target);
	ClassDB::bind_method(D_METHOD("set_idName", "value"), &drop::set_idName);
	ClassDB::bind_method(D_METHOD("get_idName"), &drop::get_idName);

	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::STRING, "idName"), "set_idName", "get_idName");
}

