#version 460 core 
        
in vec4 a_position; 
in vec2 a_texcoord; 

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform float u_frameTime; 
uniform vec3 u_scrollSpeeds;
uniform vec3 u_scales; 

out vec2 a_texcoord_out; 
out vec2 a_texCoords1; 
out vec2 a_texCoords2; 
out vec2 a_texCoords3; 

void main(void) 
{ 
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position; 
    a_texcoord_out = a_texcoord; 

    a_texCoords1 = (a_texcoord * u_scales.x); 
    a_texCoords1.y = a_texCoords1.y + (u_frameTime * u_scrollSpeeds.x); 
            
    a_texCoords2 = (a_texcoord * u_scales.y); 
    a_texCoords2.y = a_texCoords2.y + (u_frameTime * u_scrollSpeeds.y); 

    a_texCoords3 = (a_texcoord * u_scales.z); 
    a_texCoords3.y = a_texCoords3.y + (u_frameTime * u_scrollSpeeds.z); 
}
