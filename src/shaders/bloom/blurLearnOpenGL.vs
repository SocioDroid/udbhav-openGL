#version 460 core

in vec4 a_position;
in vec2 a_texcoord;

out vec2 a_texcoord_out;

void main(void)
{
    a_texcoord_out = a_texcoord;
    gl_Position = vec4(vec3(a_position), 1.0);
}
