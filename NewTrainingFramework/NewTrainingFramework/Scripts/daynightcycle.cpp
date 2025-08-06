
#include "daynightcycle.hpp"

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void daynightcycle::_ready()
{
	point_light_2d_2 = get_node("../Objects/Player/PointLight2D2");

	// Replace with function body.

}

void daynightcycle::_process(double delta)
{
	time += delta * 0.3;
	double value = (Math::sin(time - Math::Pi / 2) + 1.0) / 2.0;
	this->color = gradient->get_gradient()->sample(value);
	point_light_2d_2->set_energy(1 - value);
}

void daynightcycle::set_gradient(Ref<GradientTexture1D> value) {
	gradient = value;
}

Ref<GradientTexture1D> daynightcycle::get_gradient() {
	return gradient;
}

void daynightcycle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_gradient", "value"), &daynightcycle::set_gradient);
	ClassDB::bind_method(D_METHOD("get_gradient"), &daynightcycle::get_gradient);

	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::OBJECT, "gradient"), "set_gradient", "get_gradient");
}

