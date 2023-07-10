#version 460 core

in vec2 a_texcoords_out;

#define PI 3.14159265358979323846
#define HALFPI PI * 0.5
#define WHITE vec4(0.0,0.0,0.0,1.0)

// #define resX 1920.0
// #define resY 1080.0

uniform sampler2D u_textureSampler;
uniform float u_alphaVal = 0.0;
uniform float u_iTime;
uniform float resX;
uniform float resY;
uniform float u_clearCMix;

out vec4 FragColor;

float rand(vec2 c){
    return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void)
{
    // Parameters
    float p = 1.5; // Distance
    float a = -0.5 * PI; // Angle
    float v = 0.7; // Velocity
    float s = smoothstep(-HALFPI, HALFPI, sin(u_iTime * v));
    vec2 uv = gl_FragCoord.xy / vec2(resX, resY);
    vec2 f = vec2(sin(a) * s, cos(a) * s);
    vec4 clearC;
    vec4 c;
    clearC = texture(u_textureSampler, uv);
    
    float r = rand(uv);
    uv.x -= r * f.x * p * s;
    uv.y -= r * f.y * p * s;
    c = texture(u_textureSampler, uv);
    vec2 n = normalize(uv.xy - f);
    float d = dot(f, n);
    
    // Radial transition
    float radialMask = smoothstep(0.0, 1.0, length(uv - vec2(0.5)) / 0.5);
    c = mix(c, WHITE, s * radialMask); // c.a = s
    
    if (u_iTime >= 5.38f)
    {
        c = mix(c, clearC, u_clearCMix);
    }
    
    FragColor = vec4(c.xyz, c.a * u_alphaVal);
}

