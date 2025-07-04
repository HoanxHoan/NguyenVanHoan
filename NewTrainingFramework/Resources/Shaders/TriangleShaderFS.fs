#version 300 es
precision mediump float;
layout(location = 0) out mediump vec4 o_color;
uniform sampler2D u_texture;
in mediump vec2 v_uv;
void main()
{
	o_color = texture(u_texture, v_uv);
}
