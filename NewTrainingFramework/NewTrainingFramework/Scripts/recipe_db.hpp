
#ifndef RECIPE_DB_H
#define RECIPE_DB_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class recipe_db : public Node {
	GDCLASS(recipe_db, Node);
public:

protected:
	const Dictionary RECIPE = Dictionary {/* initializer lists are unsupported */ 
		{"wooden_workbench", Dictionary {/* initializer lists are unsupported */ 
					{"ingredients", Array {/* initializer lists are unsupported */ 
									Dictionary {/* initializer lists are unsupported */ {"item", "wood_log"},{"amount", 4}, }, 
									 }},
					{"station", ""},
					 }},
		{"stone_sword", Dictionary {/* initializer lists are unsupported */ 
					{"ingredients", Array {/* initializer lists are unsupported */ 
									Dictionary {/* initializer lists are unsupported */ {"item", "wood_log"},{"amount", 1}, }, 
									Dictionary {/* initializer lists are unsupported */ {"item", "stone"},{"amount", 3}, }, 
									 }},
					{"station", "wooden_workbench"},
					 }},

		{"stone_axe", Dictionary {/* initializer lists are unsupported */ 
					{"ingredients", Array {/* initializer lists are unsupported */ 
									Dictionary {/* initializer lists are unsupported */ {"item", "wood_log"},{"amount", 2}, }, 
									Dictionary {/* initializer lists are unsupported */ {"item", "stone"},{"amount", 3}, }, 
									 }},
					{"station", "wooden_workbench"},
					 }},

		{"stone_pickaxe", Dictionary {/* initializer lists are unsupported */ 
					{"ingredients", Array {/* initializer lists are unsupported */ 
									Dictionary {/* initializer lists are unsupported */ {"item", "wood_log"},{"amount", 3}, }, 
									Dictionary {/* initializer lists are unsupported */ {"item", "stone"},{"amount", 3}, }, 
									 }},
					{"station", "wooden_workbench"},
					 }},
		 };

};

#endif // RECIPE_DB_H
