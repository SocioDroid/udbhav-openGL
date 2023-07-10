#version 460 core

in vec4 a_color_out;
in vec3 v;
layout (location = 0) out vec4 FragColor;
void main(void)
{
FragColor = vec4(a_color_out.xyz, v.z);
}
