#version 460 core

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

in vec3 a_tangent;
in vec3 a_bittangent;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform vec4 u_plane;

out vec2 a_texcoord_out;
out vec3 FragPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

// out float gl_ClipDistance[1];

void main(void)
{
    FragPos = vec3(u_modelMatrix * a_position); // worldPosition

    a_texcoord_out = a_texcoord;
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));

    vec3 T = normalize(normalMatrix * vec3(0.0,1.0,0.0));
    vec3 N = normalize(normalMatrix * vec3(0.0,1.0,0.0));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N)); 

    TangentLightPos = TBN * lightPosition;
    TangentViewPos  = TBN * viewPosition;
    TangentFragPos  = TBN * FragPos;

    //gl_ClipDistance[0] = dot(u_modelMatrix * a_position, u_plane);

    gl_Position =  a_position;

    // // Define a clipping plane equation
    // vec4 clipPlane = vec4(1.0, 0.0, 0.0, -0.5);  // Example: Plane equation x = 0.5

    // // Compute the signed distance to the clipping plane
    // gl_ClipDistance[0]  = dot(gl_Position, clipPlane);

    // gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;

}
