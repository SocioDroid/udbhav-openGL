#version 460 core

uniform sampler2D u_textureSampler;
uniform vec2 direction;

out vec4 FragColor;
vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
    vec4 color = vec4(0.0);
    vec2 off1 = vec2(1.3846153846) * direction;
    vec2 off2 = vec2(3.2307692308) * direction;
    color += texture(image, uv) * 0.2270270270;
    color += texture(image, uv + (off1 / resolution)) * 0.3162162162;
    color += texture(image, uv - (off1 / resolution)) * 0.3162162162;
    color += texture(image, uv + (off2 / resolution)) * 0.0702702703;
    color += texture(image, uv - (off2 / resolution)) * 0.0702702703;
    return color;
}

void main(void) {
    vec2 iResolution = vec2(1920., 1080.);
    vec2 uv = vec2(gl_FragCoord.xy / iResolution);

    FragColor = blur9(u_textureSampler, uv, iResolution, direction);

}
