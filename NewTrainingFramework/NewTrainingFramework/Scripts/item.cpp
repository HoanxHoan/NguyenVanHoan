
#include "item.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void item::_ready()
{
	item_texture = get_node("CenterContainer") / ItemTexture;
	item_quantity = get_node("ItemQuantity");
	int rand_val = UtilityFunctions::randi() % 5;
	switch(rand_val)
	{
		case 0:
		{
			idName = "wood_log";
			break; }
		case 1:
		{
			idName = "cooked_beef";
			break; }
		case 2:
		{
			idName = "wooden_pickaxe";
			break; }
		case 3:
		{
			idName = "wooden_sword";
			break; }
		case 4:
		{
			idName = "wooden_axe";
			break; }
	}
	load_item(idName);
	Variant stackSize = int(item_data.stack_size);
	item_amount = UtilityFunctions::randi() % stackSize + 1;
	if(stackSize == 1)
	{
		item_quantity->set_visible(false);
	}
	else
	{
		item_quantity->set_text(str(item_amount));
	}
}

void item::load_item(String id)
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
	item_texture->set_texture(load(texture_path + icon_name + ".png"));
}

void item::_process(double delta)
{

}

void item::add_item_quantity(Variant amount_to_add)
{
	item_amount += amount_to_add;
	item_quantity->set_text(str(item_amount));
	item_quantity->set_visible(item_amount > 1);
}

void item::decrease_item_quantity(Variant amount_to_decrease)
{
	item_amount -= amount_to_decrease;
	item_quantity->set_text(str(item_amount));
	item_quantity->set_visible(item_amount > 1);
}

void item::set_item(Variant iN, Variant iA)
{
	idName = iN;
	item_amount = iA;

	load_item(idName);
	Variant stackSize = int(item_data.stack_size);
	if(stackSize == 1)
	{
		item_quantity->set_visible(false);
	}
	else
	{
		item_quantity->set_visible(true);
		item_quantity->set_text(str(item_amount));
	}
}

void item::set_idName(String value) {
	idName = value;
}

String item::get_idName() {
	return idName;
}

void item::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_item", "id"), &item::load_item);
	ClassDB::bind_method(D_METHOD("add_item_quantity", "amount_to_add"), &item::add_item_quantity);
	ClassDB::bind_method(D_METHOD("decrease_item_quantity", "amount_to_decrease"), &item::decrease_item_quantity);
	ClassDB::bind_method(D_METHOD("set_item", "iN", "iA"), &item::set_item);
	ClassDB::bind_method(D_METHOD("set_idName", "value"), &item::set_idName);
	ClassDB::bind_method(D_METHOD("get_idName"), &item::get_idName);

	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::STRING, "idName"), "set_idName", "get_idName");
}

