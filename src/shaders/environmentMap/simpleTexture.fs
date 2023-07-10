#version 460 core

//IN

in vec2 a_texcoord_out;

//UNIFORM
uniform sampler2D u_textureSampler;

//OUT

out vec4 FragColor;


void main(void)
{
	FragColor=texture(u_textureSampler,a_texcoord_out);
	
}

