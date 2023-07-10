#version 460 core

in vec4 outColor;

out vec4 FragColor;

void main(void)
{
    // code
    if(dot(gl_PointCoord - 0.5, gl_PointCoord - 0.5) > 0.25)
        discard;

    else
        FragColor = outColor;
}



