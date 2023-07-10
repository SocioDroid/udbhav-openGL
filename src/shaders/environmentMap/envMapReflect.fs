//envMapReflects.FS

#version 460 core

//IN

in vec3 hc_Position;
in vec3 hc_Normal;

//UNIFORM

uniform samplerCube u_Skybox;
uniform vec3 u_CameraPos;

//OUT

out vec4 FragColor;

void main(void)
{
	//code:
	
	vec3 i=normalize(hc_Position-u_CameraPos);
	vec3 r=reflect(i,normalize(hc_Normal));
	
	FragColor=vec4(texture(u_Skybox,r).rgb,1.0);
	
}

