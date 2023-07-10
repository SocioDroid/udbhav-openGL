#version 460 core

in vec2 a_texcoords_out;
uniform sampler2D u_textureSamplerUniform;

out vec4 FragColor;

void main(void)
{
    FragColor = vec4(texture(u_textureSamplerUniform, a_texcoords_out) * 0.5f);
}


