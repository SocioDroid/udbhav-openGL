//surfaceReflect.VS

#version 460 core

//IN

in vec4 a_position;

//UNIFORM

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

//OUT

out vec2 a_texcoords_out;
out vec4 clipSpaceCoords;

void main(void)
{
	//code:
	clipSpaceCoords=u_projectionMatrix*u_viewMatrix*u_modelMatrix*vec4(a_position);
	
	gl_Position=clipSpaceCoords;	
	a_texcoords_out=vec2(a_position.x,a_position.y);

}

