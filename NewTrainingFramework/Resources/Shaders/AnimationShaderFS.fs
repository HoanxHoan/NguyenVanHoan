#version 300 es
precision mediump float;

in vec2 v_uv;

uniform sampler2D u_texture;
uniform float u_alpha;

out vec4 fragColor;

void main()
{
    vec4 texColor = texture(u_texture, v_uv);


    texColor.a *= u_alpha;


    fragColor = texColor;
}
