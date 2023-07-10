//skyBox.VS

#version 460 core

//IN

in vec4 a_position;

//UNIFORM

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

//OUT

out vec3 hc_texcoords;

void main(void)
{
	//code:
	
	hc_texcoords=vec3(a_position);
	gl_Position=u_projectionMatrix*u_viewMatrix*u_modelMatrix*a_position;

}
