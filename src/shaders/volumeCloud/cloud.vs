#version 460 core

in vec3 a_position;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 u_cameraPos;

out vec3 vOrigin;
out vec3 vDirection;

void main() {
    vec4 mvPosition = u_viewMatrix * u_modelMatrix * vec4( a_position, 1.0 );

    vOrigin = vec3( inverse( u_modelMatrix ) * vec4( u_cameraPos, 1.0 ) ).xyz;
    vDirection = a_position - vOrigin;

    gl_Position = u_projectionMatrix * mvPosition;
}
