#version 460 core

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

uniform mat4 u_modelMatrix;

out vec2 a_texcoord_out;
out vec3 FragPos;
out vec3 Normal_CS_in; 

void main(void)
{
    Normal_CS_in  = a_normal;   
    FragPos = vec3(u_modelMatrix * a_position); // worldPosition
    a_texcoord_out = a_texcoord;

    gl_Position =  a_position;

}
