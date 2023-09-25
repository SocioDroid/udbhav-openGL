#version 460 core

in vec2 a_texcoords_out;
in vec4 phong_vertex_position;
in vec3 phong_vertex_normal_vector;
in vec3 phong_vertex_color;

uniform sampler2D u_textureSampler;
uniform sampler2D u_textureNormalSampler;
uniform bool isNormalMap = false;

uniform float u_alphaVal = 1.0;
uniform float lightX;
uniform float lightY;
uniform float lightZ;

uniform bool isMultiTexture = false;
uniform float multiTextureInterpolation = 0.0;
uniform float u_time;
uniform bool isLight = true;

out vec4 FragColor;
#define M_PI 3.14159265359

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
    return mod289(((x * 34.0) + 1.0) * x);
}

vec4 taylorInvSqrt(vec4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v) {
    const vec2 C = vec2(1.0 / 6.0, 1.0 / 3.0);
    const vec4 D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i = floor(v + dot(v, C.yyy));
    vec3 x0 = v - i + dot(i, C.xxx);

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy);
    vec3 i2 = max(g.xyz, l.zxy);

    //   x0 = x0 - 0.0 + 0.0 * C.xxx;
    //   x1 = x0 - i1  + 1.0 * C.xxx;
    //   x2 = x0 - i2  + 2.0 * C.xxx;
    //   x3 = x0 - 1.0 + 3.0 * C.xxx;
    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
    vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

    // Permutations
    i = mod289(i);
    vec4 p = permute(permute(permute(i.z + vec4(0.0, i1.z, i2.z, 1.0)) + i.y + vec4(0.0, i1.y, i2.y, 1.0)) + i.x + vec4(0.0, i1.x, i2.x, 1.0));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float n_ = 0.142857142857; // 1.0/7.0
    vec3 ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_);    // mod(j,N)

    vec4 x = x_ * ns.x + ns.yyyy;
    vec4 y = y_ * ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);

    //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
    //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
    vec4 s0 = floor(b0) * 2.0 + 1.0;
    vec4 s1 = floor(b1) * 2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    vec3 p0 = vec3(a0.xy, h.x);
    vec3 p1 = vec3(a0.zw, h.y);
    vec3 p2 = vec3(a1.xy, h.z);
    vec3 p3 = vec3(a1.zw, h.w);

    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    return 42.0 * dot(m * m, vec4(dot(p0, x0), dot(p1, x1), dot(p2, x2), dot(p3, x3)));
}

float fbm4(vec3 p, float theta, float f, float lac, float r) {
    mat3 mtx = mat3(cos(theta), -sin(theta), 0.0, sin(theta), cos(theta), 0.0, 0.0, 0.0, 1.0);

    float frequency = f;
    float lacunarity = lac;
    float roughness = r;
    float amp = 1.0;
    float total_amp = 0.0;

    float accum = 0.0;
    vec3 X = p * frequency;
    accum += amp * snoise(X);
    X *= (lacunarity + (snoise(X) + 0.1) * 0.006);
    X = mtx * X;

    total_amp += amp;
    amp *= roughness;

    return accum / total_amp;
}

float turbulence(float val) {
    float n = 1.0 - abs(val);
    return n * n;
}

float pattern(in vec3 p, inout vec3 q, inout vec3 r) {
    q.x = fbm4(p + 0.0, 0.0, 1.0, 2.0, 0.33);
    q.y = fbm4(p + 6.0, 0.0, 1.0, 2.0, 0.33);

    r.x = fbm4(p + q - 2.4, 2.0, 1.0, 2.0, 0.5);
    r.y = fbm4(p + q + 8.2, 02.0, 1.0, 2.0, 0.5);

    q.x = turbulence(q.x);
    q.y = turbulence(q.y);

    float f = fbm4(p + (1.0 * r), 0.0, 1.0, 2.0, 0.5);

    return f;
}

void main(void) {
     // LIGHT
    /* Distance to light source. */
    const float light_distance = 5.0;
    vec4 FragColorLight;
    if(isLight) {
        vec3 light_location = vec3(lightX, lightY, -1.0 + lightZ);

    /* Scene ambient color. */
        vec3 ambient_color = vec3(0.1, 0.1, 0.1);
        ;
        const float attenuation = 1.0;
        const float shiness = 1.0;

    /* Normalize directions. */
        vec3 normal_direction;

        if(multiTextureInterpolation >= 1.0) {
            vec3 normal = texture(u_textureNormalSampler, a_texcoords_out).rgb;
            normal_direction = normalize(normal * 2.0 + 1.0);
            ambient_color = vec3(1.0, 1.0, 1.0);
        } else
            normal_direction = normalize(phong_vertex_normal_vector);

        vec3 view_direction = normalize(vec3(vec4(0.0, 0.0, -5.0, 0.0) - phong_vertex_position));
        vec3 light_direction = normalize(light_location);

    /** Calculate ambient lighting component of directional light. */
        vec3 ambient_lighting = ambient_color * phong_vertex_color;

    /** Calculate diffuse reflection lighting component of directional light. */
        vec3 diffuse_reflection = attenuation * phong_vertex_color * max(0.0, dot(normal_direction, light_direction));

    /** Calculate specular reflection lighting component of directional light. */
        vec3 specular_reflection = vec3(0.0, 0.0, 0.0);

        if(dot(normal_direction, light_direction) >= 0.0) {
        /* Light source on the right side. */
            specular_reflection = attenuation * phong_vertex_color * pow(max(0.0, dot(reflect(-light_direction, normal_direction), view_direction)), shiness);
        }

    /** Calculate fragment lighting as sum of previous three component. */
        FragColorLight = vec4(ambient_lighting + diffuse_reflection + specular_reflection, 1.0);
    }

    vec4 texColor = texture(u_textureSampler, a_texcoords_out);
    if(isMultiTexture == true) {
        vec2 iResolution = vec2(1920., 1080.);
        vec2 uv = a_texcoords_out;

        float t = u_time * 0.1;

        vec3 spectrum[4];
        spectrum[0] = vec3(1.00, 1.00, 0.00);
        spectrum[1] = vec3(0.50, 0.00, 0.00);
        spectrum[2] = vec3(1.00, 0.20, 0.00);
        spectrum[3] = vec3(1.00, 0.30, 0.00);

        uv -= 0.5;
        uv -= 10. / iResolution.xy;
        uv *= 150.;

        vec3 p = vec3(uv.x, uv.y, t);
        vec3 q = vec3(0.0);
        vec3 r = vec3(0.0);
        vec3 brigth_q = vec3(0.0);
        vec3 brigth_r = vec3(0.0);
        vec3 black_q = vec3(0.0);
        vec3 black_r = vec3(0.0);
        vec3 p2 = vec3(p.xy * 0.08, p.z * 0.2);

        float black = pattern(p2, black_q, black_r);
        black = smoothstep(1.0, 0.4, length(black_q * black));

        float brigth = pattern(p2 * 2., brigth_q, brigth_r);
        brigth = smoothstep(0.0, 1.8, brigth * length(brigth_q));

        p += min(length(brigth_q), length(black_q)) * 5.;

        float f = pattern(p, q, r);

        vec3 color = vec3(0.0);
        color = mix(spectrum[1], spectrum[3], pow(length(q), 2.0));
        color = mix(color, spectrum[3], pow(length(r), 1.4));

        color = pow(color, vec3(2.0));

        vec4 sunColor = vec4(pow(black, 2.) * (color + spectrum[2] * brigth * 5.), 1.0);

        texColor = mix(sunColor, texColor, multiTextureInterpolation);
    }

    if(isLight)
        FragColor = FragColorLight * vec4(texColor.xyz, texColor.a * u_alphaVal);
    else
        FragColor = vec4(texColor.xyz, texColor.a * u_alphaVal);

}
