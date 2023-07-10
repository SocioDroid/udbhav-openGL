#version 460 core

in vec2 a_texcoord_out;

uniform sampler2D u_textureSampler;
uniform float resolution;
uniform float radius;
uniform vec2 direction;
uniform bool horizontal;
float weight[5];// = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

out vec4 FragColor;
void main()
{
    float blur = radius/resolution;
    float hstep = direction.x;
    float vstep = direction.y;

    vec4 sum = texture( u_textureSampler , vec2( a_texcoord_out.x - 4.0*blur*hstep , a_texcoord_out.y - 4.0*blur*vstep )) * 0.0162162162;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x - 3.0*blur*hstep , a_texcoord_out.y - 3.0*blur*vstep )) * 0.0540540541;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x - 2.0*blur*hstep , a_texcoord_out.y - 2.0*blur*vstep )) * 0.1216216216;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x - 1.0*blur*hstep , a_texcoord_out.y - 1.0*blur*vstep )) * 0.1945945946;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x , a_texcoord_out.y )) * 0.2270270270;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x + 1.0*blur*hstep , a_texcoord_out.y + 1.0*blur*vstep )) * 0.1945945946;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x + 2.0*blur*hstep , a_texcoord_out.y + 2.0*blur*vstep )) * 0.1216216216;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x + 3.0*blur*hstep , a_texcoord_out.y + 3.0*blur*vstep )) * 0.0540540541;
    sum += texture( u_textureSampler , vec2( a_texcoord_out.x + 4.0*blur*hstep , a_texcoord_out.y + 4.0*blur*vstep )) * 0.0162162162;

    // FragColor = sum;
    FragColor = vec4( sum.rgb , 1.0 );
// FragColor = vec4(result, 1.0);
// FragColor = vec4(1.0,0.0,0.0, 1.0);
}
