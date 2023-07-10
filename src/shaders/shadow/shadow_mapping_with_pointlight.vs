#version 460 core

in vec4 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

out vec4 a_position_out;
out vec3 a_normal_out;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_viewMatrix_for_spotlgiht;
uniform mat4 u_modelMatrix;
uniform mat4 u_lightSpaceMatrix;

void main()
{
    FragPos = vec3(u_modelMatrix * aPos);
    Normal = transpose(inverse(mat3(u_modelMatrix))) * aNormal;

    mat3 normalMatrix = mat3(u_viewMatrix_for_spotlgiht * u_modelMatrix);
	a_normal_out = normalize(normalMatrix * aNormal);


    TexCoords = aTexCoords;
    FragPosLightSpace = u_lightSpaceMatrix * vec4(FragPos, 1.0);
    
    a_position_out = u_viewMatrix_for_spotlgiht * u_modelMatrix * aPos;

    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * aPos;
}
