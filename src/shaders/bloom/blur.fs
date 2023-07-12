#version 460 core

in vec2 a_texcoord_out;

uniform sampler2D u_textureSampler;
uniform float resolution;
uniform float radius;
uniform vec2 direction;
uniform bool horizontal;
float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

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

void main() {
    // v1
    // float blur = radius / resolution;
    // float hstep = direction.x;
    // float vstep = direction.y;

    // vec4 sum = texture(u_textureSampler, vec2(a_texcoord_out.x - 4.0 * blur * hstep, a_texcoord_out.y - 4.0 * blur * vstep)) * 0.0162162162;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x - 3.0 * blur * hstep, a_texcoord_out.y - 3.0 * blur * vstep)) * 0.0540540541;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x - 2.0 * blur * hstep, a_texcoord_out.y - 2.0 * blur * vstep)) * 0.1216216216;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x - 1.0 * blur * hstep, a_texcoord_out.y - 1.0 * blur * vstep)) * 0.1945945946;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x, a_texcoord_out.y)) * 0.2270270270;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x + 1.0 * blur * hstep, a_texcoord_out.y + 1.0 * blur * vstep)) * 0.1945945946;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x + 2.0 * blur * hstep, a_texcoord_out.y + 2.0 * blur * vstep)) * 0.1216216216;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x + 3.0 * blur * hstep, a_texcoord_out.y + 3.0 * blur * vstep)) * 0.0540540541;
    // sum += texture(u_textureSampler, vec2(a_texcoord_out.x + 4.0 * blur * hstep, a_texcoord_out.y + 4.0 * blur * vstep)) * 0.0162162162;

    // FragColor = vec4(sum.rgb, 1.0);

    // V2
    // vec2 tex_offset = 1.0 / textureSize(u_textureSampler, 0); // gets size of single texel
    // vec3 result = texture(u_textureSampler, a_texcoord_out).rgb * weight[0];
    // if(horizontal) {
    //     for(int i = 1; i < 5; ++i) {
    //         result += texture(u_textureSampler, a_texcoord_out + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    //         result += texture(u_textureSampler, a_texcoord_out - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    //     }
    // } else {
    //     for(int i = 1; i < 5; ++i) {
    //         result += texture(u_textureSampler, a_texcoord_out + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    //         result += texture(u_textureSampler, a_texcoord_out - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    //     }
    // }

    // FragColor = vec4(result, 1.0);
    
    vec2 iResolution = vec2(1920., 1080.);
    vec2 uv = a_texcoord_out;

    if (!horizontal) {
        uv.y = 1.0 - uv.y;
    }

    FragColor = blur9(u_textureSampler, uv, iResolution, direction);

}
