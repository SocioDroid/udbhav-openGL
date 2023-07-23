#version 460 core

precision highp float;
precision highp sampler2D;
uniform sampler2D u_sceneWithoutBloom;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform float u_gamma = 0.0;

void main() {
    const float gamma = 2.2 + u_gamma;
    // const float bloomStrength = 2.4; // can be used as uniform
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

    hdrColor += bloomColor; // * bloomStrength; // additive blending

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0) + texture(u_sceneWithoutBloom, TexCoords);
}
