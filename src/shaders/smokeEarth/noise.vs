#version 460 core

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 a_texcoords_out;
out vec3 a_normal_out;

void main(void) {
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
	a_texcoords_out = a_texcoord;
	a_normal_out = a_normal;
}
