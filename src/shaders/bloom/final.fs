#version 460 core

precision highp float;
precision highp sampler2D;
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform sampler2D u_sceneWithoutBloom;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    // const float bloomStrength = 2.4; // can be used as uniform
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    // if(bloom)
    // {
        hdrColor += bloomColor; // * bloomStrength; // additive blending
        // hdrColor += mix(hdrColor, bloomColor, bloomStrength); // linear interpolation
    // }
    // // tone mapping method 2
    // // float YD = exposure * (exposure / 2.0 + 1.0);
    // // YD = YD / (exposure + 1.0);
    // // hdrColor *= YD;
    // // // FragColor = vec4(10.0 * hdrColor, 1.0);
    // // FragColor = vec4(5.0 * hdrColor, 1.0);

    // // tone mapping method 3
    // vec4 color = exposure * vec4(hdrColor , 1.0);
    // float tmp = (pow(exposure, 0.7)) / (pow(exposure, 0.7) + pow(20.0, 0.7));
    // FragColor = 5.0 * color * tmp;
     vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(1.0);
    
}

// // tone mapping method 1
    // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // // also gamma correct while we're at it       
    // result = pow(result, vec3(1.0 / gamma));
    // FragColor = vec4(result, 1.0);