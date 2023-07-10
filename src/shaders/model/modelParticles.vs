#version 460 core
			
in vec4 a_position;
in vec2 a_texcoord; 
in vec4 a_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 a_texcoord_out;  
out vec4 a_color_out;

const float radius = 1.0;

void main(void)
{
    a_texcoord_out = a_texcoord;
    gl_Position = (u_projectionMatrix * u_viewMatrix * u_modelMatrix  *  a_position ) ;
    
    // gl_PointSize = 5.0;
    // Controlling PointSize based on viewing distance
    a_color_out = a_color;

    gl_PointSize = 5.0 * u_projectionMatrix[1][1] * radius / gl_Position.w;

}
