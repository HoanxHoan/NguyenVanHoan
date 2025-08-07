
#ifndef DAYNIGHTCYCLE_H
#define DAYNIGHTCYCLE_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/canvas_modulate.hpp>
#include <godot_cpp/classes/gradient_texture1_d.hpp>
#include <godot_cpp/classes/point_light2d.hpp>

using namespace godot;

class daynightcycle : public CanvasModulate {
	GDCLASS(daynightcycle, CanvasModulate);
public:

protected:
	Ref<GradientTexture1D> gradient;
	Ref<PointLight2D> point_light_2d_2;

	double time = 0.0;

// Called when the node enters the scene tree for the first time.
// Called every frame. 'delta' is the elapsed time since the previous frame.

public:
	void _ready() override;

	void _process(double delta) override;
	void set_gradient(Ref<GradientTexture1D> value);
	Ref<GradientTexture1D> get_gradient();

	static void _bind_methods();
};

#endif // DAYNIGHTCYCLE_H
