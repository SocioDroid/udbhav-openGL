#version 460 core

in vec3 a_position;
in vec4 a_color;

uniform mat4 u_mvpMatrix;

out vec4 a_color_out;

void main(void)
{
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);

	a_color_out = a_color;
}
