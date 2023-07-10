#version 460 core

precision highp float;
precision highp sampler3D;

in vec3 vOrigin;
in vec3 vDirection;

out vec4 FragColor;

uniform vec3 u_base;
uniform sampler3D u_map;

uniform float u_threshold;
uniform float u_range;
uniform float u_opacity;
uniform float u_steps;
uniform float u_frame;
uniform float u_randomNum;

uint wang_hash(uint seed)
{
        seed = (seed ^ 61u) ^ (seed >> 16u);
        seed *= 9u;
        seed = seed ^ (seed >> 4u);
        seed *= 0x27d4eb2du;
        seed = seed ^ (seed >> 15u);
        return seed;
}

float randomFloat(inout uint seed)
{
        return float(wang_hash(seed)) / 4294967296.;
}

vec2 hitBox( vec3 orig, vec3 dir ) {
    const vec3 box_min = vec3( - 0.5 );
    const vec3 box_max = vec3( 0.5 );
    vec3 inv_dir = 1.0 / dir;
    vec3 tmin_tmp = ( box_min - orig ) * inv_dir;
    vec3 tmax_tmp = ( box_max - orig ) * inv_dir;
    vec3 tmin = min( tmin_tmp, tmax_tmp );
    vec3 tmax = max( tmin_tmp, tmax_tmp );
    float t0 = max( tmin.x, max( tmin.y, tmin.z ) );
    float t1 = min( tmax.x, min( tmax.y, tmax.z ) );
    return vec2( t0, t1 );
}

float sample1( vec3 p ) {
    return texture( u_map, p).r * u_randomNum;
}

float shading( vec3 coord ) {
    float step = 0.01;
    return sample1( coord + vec3( - step ) ) - sample1( coord + vec3( step ) );
}

vec4 linearToSRGB( in vec4 value ) {
    return vec4( mix( pow( value.rgb, vec3( 0.41666 ) ) * 1.055 - vec3( 0.055 ), value.rgb * 12.92, vec3( lessThanEqual( value.rgb, vec3( 0.0031308 ) ) ) ), value.a );
}

void main()
{
    vec3 rayDir = normalize( vDirection );
    vec2 bounds = hitBox( vOrigin, rayDir );

    if ( bounds.x > bounds.y ) discard;

    bounds.x = max( bounds.x, 0.0 );

    vec3 p = vOrigin + bounds.x * rayDir;
    vec3 inc = 1.0 / abs( rayDir );
    float delta = min( inc.x, min( inc.y, inc.z ) );
    delta /= u_steps;

    // Jitter

    // Nice little seed from
    // https://blog.demofox.org/2020/05/25/casual-shadertoy-path-tracing-1-basic-camera-diffuse-emissive/
    uint seed = uint( gl_FragCoord.x ) * uint( 1973 ) + uint( gl_FragCoord.y ) * uint( 9277 ) + uint( u_frame ) * uint( 26699 );
    vec3 size = vec3( textureSize( u_map, 0 ) );
    float randNum = randomFloat( seed ) * 2.0 - 1.0;
    p += rayDir * randNum * ( 1.0 / size );

    //

    vec4 ac = vec4( u_base, 0.0 );

    for ( float t = bounds.x; t < bounds.y; t += delta ) {

        float d = sample1( p + 0.5 );

        d = smoothstep( u_threshold - u_range, u_threshold + u_range, d ) * u_opacity;

        float col = shading( p + 0.5 ) * 3.0 + ( ( p.x + p.y ) * 0.25 ) + 0.2;

        ac.rgb += ( 1.0 - ac.a ) * d * col;

        ac.a += ( 1.0 - ac.a ) * d;

        if ( ac.a >= 0.95 ) break;

        p += rayDir * delta;

    }

    FragColor = linearToSRGB( ac );
    //FragColor = vec4(1.0);

    if ( FragColor.a == 0.0 ) discard;

}
