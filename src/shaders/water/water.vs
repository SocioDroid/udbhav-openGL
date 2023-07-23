#version 460 core                                                                               

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_gVP;

out vec3 Normal;
out vec4 clipSpaceCoords;
out vec2 TexCoords;
out vec4 position;

void main() {
    TexCoords = a_texcoord;
    Normal = a_normal;
    position = u_modelMatrix * vec4(a_position, 1.0);
    clipSpaceCoords = u_gVP * position;
    gl_Position = clipSpaceCoords;
}
