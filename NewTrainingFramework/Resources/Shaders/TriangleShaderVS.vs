#version 300 es
layout(location = 0) in highp vec3 a_posL;
layout(location = 1) in highp vec3 a_normal;
layout(location = 2) in highp vec3 a_binormal;
layout(location = 3) in highp vec3 a_tangent;
layout(location = 4) in mediump vec2 a_uv;

out mediump vec2 v_uv;
//out mediump vec3 v_normal;   
uniform mat4 u_model;
uniform mat4 u_mvp;
void main()
{
vec4 posL = u_mvp * vec4(a_posL, 1.0);
//vec4 posL = vec4(a_posL, 2.0);
gl_Position = posL;
v_uv = a_uv;
//v_normal = mat3(u_model) * a_normal;
}
   