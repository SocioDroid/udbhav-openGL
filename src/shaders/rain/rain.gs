#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
//layout (points, max_vertices = 4) out;

//in VertexData
//{
in float texArrayIDin[];
in vec4 velocity[];
//} vertex[];

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform vec3 eyePosition;
uniform vec3 windDir;
uniform float dt;

// for light
uniform vec4 u_lightPosition;

vec3 normal = vec3(0.0, 0.0, -1.0);


out vec3 fragmentTexCoords;
out float randEnlight;
out float texArrayID;
out vec3 particlePosition;
out vec3 particleVelocity;

// for light
out vec3 transformedNormals;
out vec3 lightDirection;
out vec3 viewerVector;


// GS for billboard technique (make two triangles from point).
void main(void)                                                                         
{
    //streak size
    float height = 0.6;
    float width = height/60.0;
                                                                                 
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(eyePosition - pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * width * length(eyePosition - pos) * 0.5;

    // For light
    vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * vec4(pos, 1.0);
    mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);
    
                                                    
    //bottom left
    pos -= right;
    fragmentTexCoords.xy = vec2(0, 0);
    fragmentTexCoords.z = texArrayIDin[0];
    randEnlight = velocity[0].w;
    particlePosition = pos;
    particleVelocity = velocity[0].xyz;
    texArrayID = texArrayIDin[0];
    transformedNormals = normalMatrix * normal;                     // for light
    lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
    viewerVector = -eyeCoordinates.xyz;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(pos + (windDir*dt), 1.0);
    EmitVertex();

    //top left
    pos.y += height;
    fragmentTexCoords.xy = vec2(0, 1);
    fragmentTexCoords.z = texArrayIDin[0];
    randEnlight = velocity[0].w;
    particlePosition = pos;
    particleVelocity = velocity[0].xyz;
    texArrayID = texArrayIDin[0];
    transformedNormals = normalMatrix * normal;                     // for light
    lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
    viewerVector = -eyeCoordinates.xyz;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(pos, 1.0);
    EmitVertex();

    //bottom right
    pos.y -= height;
    pos += right;
    fragmentTexCoords.xy = vec2(1, 0);
    fragmentTexCoords.z = texArrayIDin[0];
    randEnlight = velocity[0].w;
    particlePosition = pos;
    particleVelocity = velocity[0].xyz;
    texArrayID = texArrayIDin[0];
    transformedNormals = normalMatrix * normal;                     // for light
    lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
    viewerVector = -eyeCoordinates.xyz;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(pos + (windDir*dt), 1.0);
    EmitVertex();

    //top right
    pos.y += height;
    fragmentTexCoords.xy = vec2(1, 1);
    fragmentTexCoords.z = texArrayIDin[0];
    randEnlight = velocity[0].w;
    particlePosition = pos;
    particleVelocity = velocity[0].xyz;
    texArrayID = texArrayIDin[0];
    transformedNormals = normalMatrix * normal;                     // for light
    lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;
    viewerVector = -eyeCoordinates.xyz;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(pos, 1.0);
    EmitVertex();
                                                                                    
    EndPrimitive();                                                                 
}