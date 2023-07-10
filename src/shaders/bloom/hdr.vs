#version 460 core

in vec4 a_position;
in vec2 a_texcoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 a_texcoord_out;

void main(void)
{
    //gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
    gl_Position =  a_position;

    a_texcoord_out = a_texcoord;
    
}