#version 460 core

in vec4 a_position;
in vec2 a_texcoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform float u_time;
uniform float u_waveNumber;
uniform float u_waveVelocity;
uniform float u_waveAmplitude;

out vec4 a_position_out;
out vec2 a_texcoords_out;

void main(void)
{
	vec4 pos = a_position;

	float u = u_waveNumber * ( pos.x - u_waveVelocity * u_time);
	if(pos.x > 0.05)
	{
		pos.z = u_waveAmplitude * sin(u);
	}
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * pos;
	a_texcoords_out = a_texcoord;
}

