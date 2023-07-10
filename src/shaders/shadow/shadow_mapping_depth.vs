#version 460 core

in vec4 aPos;

uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_lightSpaceMatrix * u_modelMatrix * aPos;
}
