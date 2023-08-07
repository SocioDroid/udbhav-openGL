#version 460 core                                   

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

uniform float u_time;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec4 a_color_out;

void main(void) {
    vec4 newVertex = a_position;

    newVertex.z += u_time;
    newVertex.z = fract(newVertex.z);

    float size = (20.0 * newVertex.z * newVertex.z);

    a_color_out = smoothstep(1.0, 7.0, size) * a_color;

    newVertex.z = (999.9 * newVertex.z) - 1000.0;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * newVertex;
    gl_PointSize = size;
}
