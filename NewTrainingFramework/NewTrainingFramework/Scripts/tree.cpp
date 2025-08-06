
#include "tree.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void tree::_ready()
{
	hit_flash = get_node("hit_flash");
	tree_animation = get_node("TreeAnimation");
	player = get_tree()->get_first_node_in_group("Player");
	collision = get_node("Collision");
	collision_shape_2d = get_node("hurtBox") / CollisionShape2D;
	loot_base = get_tree()->get_first_node_in_group("Drops");

	// Replace with function body.

}

void tree::_process(double delta)
{
	if(hp <= 0)
	{
		death();
	}
}

void tree::_on_hurt_box_body_entered(Ref<Node2D> body)
{
	playerpos = player->global_position;
	if(body->is_in_group("Attack"))
	{
		hp -= 1;
		hit_flash->play("hit_flash");
	}
}

void tree::death()
{
	if(playerpos.x > this->global_position.x)
	{
		tree_animation->play("get_hit");
	}
	else
	{
		tree_animation->play("hit_get");
	}
}

void tree::_on_tree_animation_animation_finished(StringName anim_name)
{
	collision->set_disabled(true);
	collision_shape_2d->set_disabled(true);

	int drop_count = UtilityFunctions::rand_range(2, 4);
	for(int i=0; i<drop_count; i+=1)
	{
		Variant new_drop = tree_drop->instantiate();

		// Tính vị trí ngẫu nhiên trong hình tròn bán kính 20
		double angle = UtilityFunctions::rand_range(0, Math::Tau);
		// TAU = 2*PI
		double radius = UtilityFunctions::rand_range(0, 20);
		double offset = Vector2(Math::cos(angle), Math::sin(angle)) * radius;

		new_drop->global_position = global_position + offset;
		new_drop->idName = "wood_log";

		loot_base->call_deferred("add_child", new_drop);
	}

	this->visible = false;
	queue_free();
}

void tree::_on_hurt_box_area_entered(Ref<Area2D> area)
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
		}

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

void tree::_bind_methods() {
	ClassDB::bind_method(D_METHOD("death"), &tree::death);

}

