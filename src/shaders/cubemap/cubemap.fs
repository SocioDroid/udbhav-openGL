#version 460 core

in vec4 a_color_out;
out vec4 FragColor;
in vec3 a_texcoords;
uniform samplerCube u_skyBox;
uniform int isBarsat = 0;
void main(void)
{
	if(isBarsat == 1)
		FragColor = texture(u_skyBox, a_texcoords) * vec4(0.6 , 0.6, 0.6,1.0);
	else
		FragColor = texture(u_skyBox, a_texcoords);
};
    