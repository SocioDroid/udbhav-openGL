#version 460 core

in vec4 a_position;
in vec2 texcoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 texcoord_out;

//out bool isLeft;

void main() 
{
	// const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0),
    //                                vec4( 1.0, -1.0, 0.5, 1.0),
    //                                vec4(-1.0,  1.0, 0.5, 1.0),
    //                                vec4( 1.0,  1.0, 0.5, 1.0));

    // gl_Position = vertices[gl_VertexID];
	texcoord_out = texcoord;

    // gl_TexCoord[0] = a_position;
    // gl_Position = a_position * 2.0 - 1.0;

	// gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;

    //if (a_position.x < 0.0)
    //    isLeft = true;
    //else
    //    isLeft = false;

	gl_Position = a_position;
} 
