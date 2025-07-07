#version 300 es
precision mediump float;
layout(location = 0) out mediump vec4 o_color;
uniform sampler2D u_texture;

in mediump vec2 v_uv;
in mediump vec3 v_normal;

void main()
{
	o_color = texture(u_texture, v_uv);
	vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));
    float diff = max(dot(normalize(v_normal), lightDir), 0.0);

}
