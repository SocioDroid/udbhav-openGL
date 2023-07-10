//simpleEnvMap.FS

#version 460 core

//IN:

in vec3 normalOUT;
in vec3 normalOUT_fromBook;
in vec3 viewOUT;

//UNIFORM:

uniform sampler2D u_envmapSampler;

//OUT:

out vec4 FragColor;


void main(void)
{
	//code:
	
	vec3 u=normalize(viewOUT);
	vec3 r=reflect(viewOUT,normalize(normalOUT_fromBook));
	
	r.z+=1.0;
	
	float m=0.5*inversesqrt(dot(r,r));
	FragColor=texture(u_envmapSampler,r.xy*m+vec2(0.5));
	
}

