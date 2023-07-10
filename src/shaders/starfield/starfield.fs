#version 460 core

layout (location = 0) out vec4 FragColor;                    

uniform sampler2D u_textureSampler;                              
in vec4 a_color_out;                                  

void main(void)                                          
{                                                        
    FragColor = a_color_out * texture(u_textureSampler, gl_PointCoord);
}
