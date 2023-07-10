#version 460 core

in vec4 a_position;
in vec4 a_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec4 a_color_out;
out vec3 v;

void main(void)
{
    vec4 P = u_viewMatrix * u_modelMatrix * a_position;
    v = -P.xyz;

    a_color_out = a_color;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
    gl_PointSize = 6.0;
}