#version 460 core

in vec2 a_texcoords_out;

uniform sampler2D u_textureSampler;
uniform float u_alphaVal = 1.0;

out vec4 FragColor;

void main(void)
{
    vec4 texColor = texture(u_textureSampler, a_texcoords_out);
    FragColor = vec4( texColor.xyz, texColor.a * u_alphaVal);
}
