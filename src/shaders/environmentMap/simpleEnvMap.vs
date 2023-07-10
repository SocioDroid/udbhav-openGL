//simpleEnvMap.VS

#version 460 core

//IN:

in vec4 a_position;
in vec4 a_normal;

//UNIFORM:

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

//OUT:

out vec3 normalOUT_fromBook;
out vec3 viewOUT;

void main(void)
{
	//code:
	
	vec4 position_MV=(u_modelMatrix*u_viewMatrix)*a_position;
	normalOUT_fromBook=mat3(u_modelMatrix*u_viewMatrix)*a_normal;
	
	viewOUT=position_MV.xyz;
	
	gl_Position=u_projectionMatrix*position_MV;

}

