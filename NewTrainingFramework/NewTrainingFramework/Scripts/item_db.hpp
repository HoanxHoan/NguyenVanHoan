
#ifndef ITEM_DB_H
#define ITEM_DB_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class item_db : public Node {
	GDCLASS(item_db, Node);
public:

protected:
	const String TOOLS_PATH = "res://Texture/Icons/Tools/";
	const String FOOD_PATH = "res://Texture/Icons/Food/";
	const String MATERIAL = "res://Texture/Icons/Materials/";

	const Dictionary ITEMS = Dictionary {/* initializer lists are unsupported */ 
		{"wood_log", Dictionary {/* initializer lists are unsupported */ 
					{"type", "material"},
					{"catagory", "log"},
					{"icon_name", "log"},
					{"display_name", "Wooden Log"},
					{"stack_size", 16},
					 }},
		{"stone", Dictionary {/* initializer lists are unsupported */ 
					{"type", "material"},
					{"catagory", "stone"},
					{"icon_name", "stone"},
					{"display_name", "Stone"},
					{"stack_size", 16},
					 }},
		{"cooked_beef", Dictionary {/* initializer lists are unsupported */ 
					{"type", "food"},
					{"catagory", "meat"},
					{"icon_name", "95_steak"},
					{"display_name", "Wooden Pickaxe"},
					{"energy", 5},
					{"stack_size", 16},
					 }},

		{"wooden_pickaxe", Dictionary {/* initializer lists are unsupported */ 
					{"type", "tools"},
					{"catagory", "pickaxe"},
					{"icon_name", "wooden_pickaxe"},
					{"display_name", "Wooden Pickaxe"},
					{"item_attack", 1.5},
					{"item_speed", 1},
					{"mining_speed", 1},
					{"stack_size", 1},
					 }},

		{"wooden_sword", Dictionary {/* initializer lists are unsupported */ 
					{"type", "tools"},
					{"catagory", "sword"},
					{"icon_name", "wooden_sword"},
					{"display_name", "Wooden Sword"},
					{"item_attack", 2.5},
					{"item_speed", 1},
					{"stack_size", 1},
					 }},

		{"wooden_axe", Dictionary {/* initializer lists are unsupported */ 
					{"type", "tools"},
					{"catagory", "axe"},
					{"icon_name", "wooden_axe"},
					{"display_name", "Wooden Axe"},
					{"item_attack", 3.5},
					{"item_speed", 1.5},
					{"chopping_speed", 1},
					{"stack_size", 1},
					 }},
		{"stone_pickaxe", Dictionary {/* initializer lists are unsupported */ 
					{"type", "tools"},
					{"catagory", "pickaxe"},
					{"icon_name", "stone_pickaxe"},
					{"display_name", "Stone Pickaxe"},
					{"item_attack", 1.5},
					{"item_speed", 1},
					{"mining_speed", 1},
					{"stack_size", 1},
					 }},

		{"stone_sword", Dictionary {/* initializer lists are unsupported */ 
					{"type", "tools"},
					{"catagory", "sword"},
					{"icon_name", "stone_sword"},
					{"display_name", "Stone Sword"},
					{"item_attack", 2.5},
					{"item_speed", 1},
					{"stack_size", 1},
					 }},

		{"stone_axe", Dictionary {/* initializer lists are unsupported */ 
					{"type", "tools"},
					{"catagory", "axe"},
					{"icon_name", "stone_axe"},
					{"display_name", "Stone Axe"},
					{"item_attack", 5},
					{"item_speed", 1.3},
					{"chopping_speed", 1},
					{"stack_size", 1},
					 }},
		{"wooden_workbench", Dictionary {/* initializer lists are unsupported */ 
					{"type", "placeable"},
					{"catagory", "station"},
					{"icon_name", "wooden_workbench"},
					{"display_name", "WorkBench"},
					{"stack_size", 1},

					 }},
		 };

};

#endif // ITEM_DB_H
