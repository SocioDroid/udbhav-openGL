#version 460 core

in vec4 a_position;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 a_texcoords_out;

void main(void)
{
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position.x, 0.0f, a_position.y, 1.0);
	a_texcoords_out = vec2(a_position.x/2.0 + 0.5, a_position.y/2.0 + 0.5) * 100.0;
}



