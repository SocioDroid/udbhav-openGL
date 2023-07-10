#version 460 core

in vec2 a_texcoord_out;  

in vec4 a_color_out;

uniform sampler2D  u_texturesampler;
uniform float u_alpha;
out vec4 FragColor;

void main(void)
{
    // if(u_alpha < 1.0)
    //     FragColor =  vec4(a_color_out.xyz, u_alpha);
    // else
        FragColor =  a_color_out * u_alpha;

}
