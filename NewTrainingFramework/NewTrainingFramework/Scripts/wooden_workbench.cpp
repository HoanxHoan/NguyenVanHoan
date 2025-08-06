
#include "wooden_workbench.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void wooden_workbench::_ready()
{
	player = get_tree()->get_first_node_in_group("Player");
	workbench_collision = get_node("WorkbenchCollision");
	collision_shape_2d = get_node("hurt_box") / CollisionShape2D;
	loot_base = get_tree()->get_first_node_in_group("Drops");
	hit_flash = get_node("hit_flash");
	crafting_area_collision = get_node("CraftingArea") / CraftingAreaCollision;

	// Replace with function body.

}

void wooden_workbench::_process(double delta)
{
	if(!is_placed)
	{
		crafting_area_collision->set_disabled(true);
	}
}

void wooden_workbench::_physics_process(double delta)
{
	if(hp <= 0)
	{
		death();
	}
}

void wooden_workbench::death()
{
	workbench_collision->set_disabled(true);
	collision_shape_2d->set_disabled(true);

	int drop_count = 1;
	for(int i=0; i<drop_count; i+=1)
	{
		Variant new_drop = drop->instantiate();

		// Tính vị trí ngẫu nhiên trong hình tròn bán kính 20
		double angle = UtilityFunctions::rand_range(0, Math::Tau);
		// TAU = 2*PI
		double radius = UtilityFunctions::rand_range(0, 20);
		double offset = Vector2(Math::cos(angle), Math::sin(angle)) * radius;

		new_drop->global_position = global_position + offset;
		new_drop->idName = "wooden_workbench";

		loot_base->call_deferred("add_child", new_drop);
	}

	this->visible = false;
	queue_free();
}

void wooden_workbench::_on_hurt_box_area_entered(Ref<Area2D> area)
{
	if(area->is_in_group("Attack"))
	{
		String weapon_id = "";
		int damage = 0;
		if(area->has_method("get_weapon_id") && area->has_method("get_weapon_damage"))
		{
			weapon_id = area->get_weapon_id();
			damage = area->get_weapon_damage();
			UtilityFunctions::print(damage);

			// Kiểm tra có phải rìu không
			if(ItemDb.ITEMS.has(weapon_id))
			{
				Variant item_data = ItemDb.ITEMS[weapon_id];
				if(item_data->get("catagory", "") == "axe")
				{
					hp -= damage;
					hit_flash->play("hit_flash");
					if(hp <= 0)
					{
						death();
					}
				}
			}
		}
	}
}

void wooden_workbench::_on_crafting_area_body_entered(Ref<Node2D> body)
{
	if(body->is_in_group("Player"))
	{
		body->standing_near.append("wooden_workbench");
	}
}

void wooden_workbench::_on_crafting_area_body_exited(Ref<Node2D> body)
{
	if(body->is_in_group("Player"))
	{
		body->standing_near.erase("wooden_workbench");
	}
}

void wooden_workbench::_bind_methods() {
	ClassDB::bind_method(D_METHOD("death"), &wooden_workbench::death);

}

