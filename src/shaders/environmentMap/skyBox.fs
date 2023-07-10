//skyBox.FS

#version 460 core

//IN

in vec3 hc_texcoords;

//UNIFORM

uniform samplerCube u_Skybox;

//OUT

out vec4 FragColor;

void main(void)
{

	FragColor=texture(u_Skybox,hc_texcoords);
}

