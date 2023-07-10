#version 460 core

in vec2 a_texcoords_out;

uniform sampler2D u_clothTextureSampler;

out vec4 FragColor;

uniform float u_alpha;

void main(void)
{
    vec4 color = texture(u_clothTextureSampler, a_texcoords_out);
    color.a = color.a * u_alpha;
    FragColor = color;
}





