#version 460 core

in vec2 a_texcoord_out;

uniform sampler2D u_textureSampler;
out vec4 FragColor;

void main(void)
{
    FragColor = texture(u_textureSampler, a_texcoord_out);
}
