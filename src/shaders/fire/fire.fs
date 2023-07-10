#version 460 core 

vec4 noise1;
vec4 noise2;
vec4 noise3;
vec4 finalNoise;
float perturb;
vec2 noiseCoords;
vec4 alphaColor;
vec4 fireColor;

in vec2 a_texcoord_out;
in vec2 a_texCoords1;
in vec2 a_texCoords2;
in vec2 a_texCoords3;

uniform vec2 u_distortion1;
uniform vec2 u_distortion2;
uniform vec2 u_distortion3;
uniform float u_distortionScale;
uniform float u_distortionBias;
        
uniform sampler2D u_fireSampler; 
uniform sampler2D u_noiseSampler;
uniform sampler2D u_alphaSampler;

out vec4 FragColor; 

void main(void) 
{ 
    noise1 = texture(u_noiseSampler, a_texCoords1); 
    noise2 = texture(u_noiseSampler, a_texCoords2); 
    noise3 = texture(u_noiseSampler, a_texCoords3); 

    noise1 = (noise1 - 0.5) * 2.0; 
    noise2 = (noise2 - 0.5) * 2.0; 
    noise3 = (noise3 - 0.5) * 2.0; 

    noise1.xy = noise1.xy * u_distortion1.xy; 
    noise2.xy = noise2.xy * u_distortion2.xy; 
    noise3.xy = noise3.xy * u_distortion3.xy; 

    finalNoise = noise1 + noise2 + noise3; 

    perturb = ((1.0 - a_texcoord_out.y) * u_distortionScale) + u_distortionBias; 

    noiseCoords.xy = (finalNoise.xy * perturb) + a_texcoord_out.xy; 

    fireColor = texture(u_fireSampler, noiseCoords.xy);
            
    alphaColor = texture(u_alphaSampler, noiseCoords.xy);

    fireColor.a = alphaColor.r;

    FragColor = fireColor;
        
}
