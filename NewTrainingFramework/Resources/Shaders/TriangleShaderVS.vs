#version 300 es
layout(location = 0) in highp vec3 a_posL;
layout(location = 1) in mediump vec2 a_uv;
out mediump vec2 v_uv;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = posL;
v_uv = a_uv;
}
   