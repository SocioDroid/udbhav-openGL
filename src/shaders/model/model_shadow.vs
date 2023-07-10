#version 440 core
			
in vec4 a_position;
in vec3 a_normal;
in vec2 a_texcoord; 
in vec3 aTangent;
in vec3 aBitangent;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec4 u_lightPosition;
uniform int u_lightingEnabled;
uniform vec3 offsets[100];
layout(binding = 1) uniform sampler2D  texture_specular1;
uniform vec4 u_plane;
uniform mat4 u_lightSpaceMatrix;

out vec2 a_texcoord_out;  
out vec3 transformedNormals;
out vec3 lightDirection;
out vec3 viewerVector;
out float gl_ClipDistance[1];
out vec3 FragPos;
out vec4 FragPosLightSpace;

void main(void)
{

    vec3 normal = texture(texture_specular1, a_texcoord).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);

    if(u_lightingEnabled == 1)
    {
    vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;
    mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
    transformedNormals = normalMatrix * a_normal;
    lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
    viewerVector = -eyeCoordinates.xyz;
    }
    a_texcoord_out = a_texcoord;

    vec4 Matrix = vec4(offsets[gl_InstanceID], 1.0);

    // Shadow Related
    FragPos = vec3(u_modelMatrix * a_position);
    FragPosLightSpace = u_lightSpaceMatrix * vec4(FragPos, 1.0);
    
    // gl_ClipDistance[0] = dot( u_modelMatrix  * (a_position + Matrix) , u_plane);
     gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;
    // gl_Position = (u_projectionMatrix * u_viewMatrix * u_modelMatrix  * (a_position + Matrix) ) ;
    // gl_ClipDistance[0] = dot( u_modelMatrix  * (a_position + Matrix) , u_plane + Matrix);
}
