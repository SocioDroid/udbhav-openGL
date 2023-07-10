#version 460 core

in vec4 a_positionMass;
in vec2 a_texcoords;

out vec2 a_texcoords_out;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform float u_textureTiling;

const vec4 s_plane = vec4(1.0, 0.0, 0.0, 0.0);
const vec4 t_plane = vec4(0.0, 1.0, 0.0, 0.0);

void main(void)
{
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_positionMass.xyz, 1.0);

	vec4 pos = u_modelMatrix * vec4(a_positionMass.xyz, 1.0);
	
	a_texcoords_out.x = dot(pos, s_plane) / u_textureTiling;
	a_texcoords_out.y = dot(pos, t_plane) / u_textureTiling;
}



