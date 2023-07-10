#version 460 core

layout(binding = 0) uniform sampler2D input_image;
layout(location = 0) out vec4 color;
uniform float focal_distance = 30.0;
uniform float focal_depth = 10.0;
void main(void)
{
    vec2 s = 1.0 / textureSize(input_image, 0);
    vec2 C = gl_FragCoord.xy;
    vec4 v = texelFetch(input_image, ivec2(gl_FragCoord.xy), 0).rgba;
    float m;
    if (v.w == 0.0)
    {
        m = 0.5;
    }
    else
    {
        m = abs(v.w - focal_distance);
        m = 0.5 + smoothstep(0.0, focal_depth, m) * 7.5;
    }
    vec2 P0 = vec2(C * 1.0) + vec2(-m, -m);
    vec2 P1 = vec2(C * 1.0) + vec2(-m, m);
    vec2 P2 = vec2(C * 1.0) + vec2(m, -m);
    vec2 P3 = vec2(C * 1.0) + vec2(m, m);
    P0 *= s;
    P1 *= s;
    P2 *= s;
    P3 *= s;
    vec3 a = textureLod(input_image, P0, 0.0).rgb;
    vec3 b = textureLod(input_image, P1, 0.0).rgb;
    vec3 c = textureLod(input_image, P2, 0.0).rgb;
    vec3 d = textureLod(input_image, P3, 0.0).rgb;
    vec3 f = a - b - c + d;
    m *= 2;
    f /= float(m * m);
    color = vec4(f, 1.0);
    // color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

}
