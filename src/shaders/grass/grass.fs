#version 460 core

// IN
in vec2 texCoord;
in float colorVariation;

// OUT
out vec4 FragColor;

// UNIFORMS
uniform sampler2D u_grassTexture;

void main(void) 
{ 
    vec4 color = texture(u_grassTexture, texCoord);
    
    // Remove Transparent Areas
    if (color.a < 0.25) 
        discard;
    
    color.xyz = mix(color.xyz, 0.5 * color.xyz, colorVariation);
	
    FragColor = color;
}
