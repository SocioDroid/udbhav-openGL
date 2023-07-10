#version 460 core

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec4 u_lightPosition;

out vec2 a_texcoords_out;
out vec3 transformedNormals;
out vec3 lightDirection;
out vec3 viewerVector;

void main(void)
{
	vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;
	mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
	transformedNormals = normalMatrix * a_normal;
	lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
	viewerVector = -eyeCoordinates.xyz;

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
	a_texcoords_out = a_texcoord;
}
