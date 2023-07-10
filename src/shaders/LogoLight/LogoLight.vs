#version 460 core
		
in vec4 a_position;
in vec3 a_normal;
in vec2 a_texcoord; 

out vec3 a_normal_out;
out vec4 a_position_out;
out vec2 a_texcoord_out;  

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform int u_lightingEnabled;

void main(void)
{
	if(u_lightingEnabled == 1)
	{
		mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
		a_normal_out = normalize(normalMatrix * a_normal);
		//a_normal_out = mat3(transpose(inverse(u_modelMatrix))) * a_normal;
//		a_position_out = u_modelMatrix * u_viewMatrix * a_position;
		a_position_out = u_viewMatrix * u_modelMatrix * a_position;
	}
    a_texcoord_out = a_texcoord;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
}




