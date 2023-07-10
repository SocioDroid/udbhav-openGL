#version 460 core

#extension GL_EXT_gpu_shader4 : require

in vec4 a_positionMass;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform float u_pointSize;
uniform int u_selectedIndex;

uniform vec4 u_color;

out vec4 outColor;

void main(void)
{
	// code
	vec4 vertex = vec4(a_positionMass.xyz, 1.0);

	mat4 modelViewMatrix = u_viewMatrix * u_modelMatrix;
	vec3 eyePosition = (modelViewMatrix * vertex).xyz;

	gl_PointSize = max(1.0, u_pointSize / (1.0 - eyePosition.z));
	
	if(u_selectedIndex == gl_VertexID)
		outColor = 1.0 - u_color;

	else
		outColor = u_color;

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_positionMass.xyz, 1.0);
}




