#version 460 core

in vec3 aPos;
in vec2 aTexCoords;

// TESTING uniforms
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 TexCoords;
void main()
{
    TexCoords = aTexCoords;
     gl_Position = vec4(aPos, 1.0);
    //gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(aPos, 1.0);
}
