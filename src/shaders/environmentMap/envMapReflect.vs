//envMapReflects.VS

#version 460 core

//IN

in vec4 a_position;
in vec4 a_normal;

//UNIFORM

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

//OUT

out vec3 hc_Normal;
out vec3 hc_Position;

void main(void)
{
	//code:

	hc_Normal=mat3(inverse(u_modelMatrix))*vec3(a_normal);
	
	hc_Position=u_projectionMatrix*u_viewMatrix*u_modelMatrix*a_position;
	
}

