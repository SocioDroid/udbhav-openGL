#version 460 core

in vec4 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
uniform mat4 u_lightSpaceMatrix;

void main()
{
    FragPos = vec3(u_modelMatrix * aPos);
    Normal = transpose(inverse(mat3(u_modelMatrix))) * aNormal;
    TexCoords = aTexCoords;
    FragPosLightSpace = u_lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * aPos;
}
