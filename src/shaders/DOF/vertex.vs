#version 460 core

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec4 a_color;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
out vec3 v;
out vec4 a_color_out;
void main(void)
{
vec4 P = u_viewMatrix * u_modelMatrix * a_position;
v = -P.xyz;
gl_Position = u_projectionMatrix * P;
a_color_out = a_color;
}



