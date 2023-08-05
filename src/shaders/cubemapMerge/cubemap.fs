#version 460 core

in vec4 a_color_out;
out vec4 FragColor;
in vec3 a_texcoords;
uniform samplerCube u_skyBox1;
uniform samplerCube u_skyBox2;
uniform float u_factor = 0.0;
void main(void)
{
		FragColor = mix(texture(u_skyBox1, a_texcoords), texture(u_skyBox2, a_texcoords), u_factor);
};

