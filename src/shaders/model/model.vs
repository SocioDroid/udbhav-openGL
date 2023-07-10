#version 460 core
			
in vec4 a_position;
in vec3 a_normal;
in vec2 a_texcoord; 

in vec3 aTangent;
in vec3 aBitangent;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec4 u_lightPosition;

uniform vec3 offsets[100];
uniform vec4 u_plane;

uniform sampler2D texture_specular1;
uniform vec3 viewPos;

out vec2 a_texcoord_out;
out vec3 transformedNormals;
out vec3 lightDirection;
out vec3 viewerVector;

out vec3 FragPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

out float gl_ClipDistance[1];
out vec3 v_DOF;

void main(void)
{
    // DOF
    vec4 P = u_viewMatrix * u_modelMatrix * a_position;
    v_DOF = -P.xyz;

    // NORMAL MAPPING
    FragPos = vec3(u_modelMatrix * a_position);   
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * a_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    TangentLightPos = TBN * vec3(u_lightPosition);
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
  
    // PER FRAG LIGHT
    vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;
    mat3 normalMatrix1 = mat3(u_viewMatrix * u_modelMatrix);
    transformedNormals = normalMatrix1 * a_normal;
    lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
    viewerVector = -eyeCoordinates.xyz;
    
    a_texcoord_out = a_texcoord;

    vec4 Matrix = vec4(offsets[gl_InstanceID], 1.0);

    // gl_ClipDistance[0] = dot( u_modelMatrix  * (a_position + Matrix) , u_plane);
     gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
    // gl_Position = (u_projectionMatrix * u_viewMatrix * u_modelMatrix  * (a_position + Matrix) ) ;
    // gl_ClipDistance[0] = dot( u_modelMatrix  * (a_position + Matrix) , u_plane + Matrix);
}
