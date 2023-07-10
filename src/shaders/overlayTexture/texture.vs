#version 460 core

in vec3 a_position;
in vec2 a_texcoord;

uniform mat4 u_mvpMatrix;

out vec2 a_texcoords_out;

void main(void)
{
	gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
	//  gl_Position = vec4(position, 1.0);
	a_texcoords_out = a_texcoord;
}
