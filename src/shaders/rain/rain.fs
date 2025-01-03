#version 460 core

#extension GL_EXT_gpu_shader4 : enable

#define PI 3.14159265
#define MAX_POINT_LIGHTS 2

in vec3 fragmentTexCoords;
in float randEnlight;
in float texArrayID;
in vec3 particlePosition;
in vec3 particleVelocity;

// for light
in vec3 transformedNormals;
in vec3 lightDirection;
in vec3 viewerVector;

// uniform sampler2DArray rainTex;
uniform sampler2D rainTex;
uniform sampler1D rainfactors;
uniform vec3 eyePosition;

//lighting parameter
uniform vec3 sunDir;
uniform vec3 sunColor;
uniform float sunIntensity;

// for Phong light
uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_materialShiness;
uniform float u_alpha = 1.0;
out vec4 FragColor;

// /**
//  * Calculate Rain response to light source.
//  * Based on Sarah Tariq'p "Rain" demo
//  * @see http://developer.download.nvidia.com/SDK/10/direct3d/Source/rain/doc/RainSDKWhitePaper.pdf
//  */
vec4 rainResponse(vec3 lightVec, vec3 lightColor, float lightIntensity, bool fallOffFactor) {
    float opacity = 0.0;
    float fallOff = 1.0;

    lightVec = lightVec - particlePosition;

    int maxVIDX = 4;
    int maxHIDX = 8;

    lightVec = normalize(lightVec);
    vec3 eyeVec = normalize(eyePosition - particlePosition);
    vec3 dropDir = normalize(vec3(0.0, -particleVelocity.y, 0.0));

    bool is_ccw = true;
    float phi = 0;
    // angle between light vector and drop direction
    float theta = abs(acos(dot(lightVec, dropDir)) * 180 / PI - 90); // 0 to 90
    // light vector projection
    vec3 lightProj = normalize(lightVec - dot(lightVec, dropDir) * dropDir);
    // eye vector projection
    vec3 eyeProj = normalize(eyeVec - dot(eyeVec, dropDir) * dropDir);
    phi = acos(dot(eyeProj, lightProj)) * 180 / PI; // 0 to 180

    phi = (phi - 10) / 20.0;                      // -0.5 to 8.5
    is_ccw = dot(dropDir, cross(eyeProj, lightProj)) > 0;

    if(theta >= 88.0) {
        phi = 0;
        is_ccw = true;
    }
    theta = (theta - 10.0) / 20.0; // -0.5 to 4

    int verticalId1 = int(floor(theta)); // -1 to 4
    int verticalId2 = int(min(maxVIDX, (verticalId1 + 1))); // 0 to 4
    float t = fract(theta); //0 to 0.999..

    float texCoordsH1 = fragmentTexCoords.x;
    float texCoordsH2 = fragmentTexCoords.x;

    int horizontalId1 = int(floor(phi)); // -1 to 8
    int horizontalId2 = horizontalId1 + 1; // 0 to 9
    float p = fract(phi); // 0 to 0.999..
    if(horizontalId1 < 0) {
        horizontalId1 = 0;
        horizontalId2 = 0;
    }

    if(is_ccw) {
        if(horizontalId2 > maxHIDX) {
            horizontalId2 = maxHIDX;
            texCoordsH2 = 1.0 - texCoordsH2;
        }
    } else {
        texCoordsH1 = 1.0 - texCoordsH1;
        if(horizontalId2 > maxHIDX) {
            horizontalId2 = maxHIDX;
        } else {
            texCoordsH2 = 1.0 - texCoordsH2;
        }
    }
    if(verticalId1 >= maxVIDX) {
        texCoordsH2 = fragmentTexCoords.x;
        horizontalId1 = 0;
        horizontalId2 = 0;
        p = 0;
    }

    // fixed for directional lighting
    verticalId2 = 3;
    verticalId1 = 0;
    // Generate final texture coordinates for each sample
    int type = int(texArrayID); // 0 to 7
    ivec2 texIdV1 = ivec2((verticalId1 * 90 + horizontalId1 * 10 + type), (verticalId1 * 90 + horizontalId2 * 10 + type));
    vec3 tex1 = vec3(texCoordsH1, fragmentTexCoords.y, texIdV1.x);
    vec3 tex2 = vec3(texCoordsH2, fragmentTexCoords.y, texIdV1.y);
    if((verticalId1 < maxVIDX) && (verticalId2 >= maxVIDX)) {
        p = 0;
        horizontalId1 = 0;
        horizontalId2 = 0;
        texCoordsH1 = fragmentTexCoords.x;
        texCoordsH2 = fragmentTexCoords.x;
    }

    ivec2 texIdV2 = ivec2((verticalId2 * 90 + horizontalId1 * 10 + type), (verticalId2 * 90 + horizontalId2 * 10 + type));
    vec3 tex3 = vec3(texCoordsH1, fragmentTexCoords.y, texIdV2.x);
    vec3 tex4 = vec3(texCoordsH2, fragmentTexCoords.y, texIdV2.y);

    float col1 = texture(rainTex, vec2(tex1)).r;//* texelFetch(rainfactors, texIdV1.x, 0).r;
    float col2 = texture(rainTex, vec2(tex2)).r;//* texelFetch(rainfactors, texIdV1.y, 0).r;
    float col3 = texture(rainTex, vec2(tex3)).r;//* texelFetch(rainfactors, texIdV2.x, 0).r;
    float col4 = texture(rainTex, vec2(tex4)).r;//* texelFetch(rainfactors, texIdV2.y, 0).b;

    // Compute interpolated opacity
    float hOpacity1 = mix(col1, col2, p);
    float hOpacity2 = mix(col3, col4, p);
    opacity = mix(hOpacity1, hOpacity2, 0.9);
    // gamma correction
    opacity = pow(opacity, 0.7);
    opacity *= 0.7 * lightIntensity * fallOff;
    return vec4(lightColor, opacity);
}

vec3 lightFunc(void) {
    vec3 phong_ads_light;
    vec3 ambient = u_la * u_ka;
    vec3 normalized_transformed_normals = normalize(transformedNormals);
    vec3 normalized_light_direction = normalize(lightDirection);
    vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0);
    vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals);
    vec3 normalized_viewervector = normalize(viewerVector);
    vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewervector), 0.0), u_materialShiness);

    return phong_ads_light = ambient + diffuse + specular;
}

void main(void) {

    //TODO: point lighting
    vec4 pointLight = vec4(1.0, 1.0, 1.0, 0.5);
    vec4 pointLightColor = vec4(1.0, 1.0, 1.0, 1.0);
    float pointLightIntensity = 2.0;
    vec3 pointLightDir = vec3(0, -1, 0);

    vec3 lightDir = normalize(pointLightDir);
    float angleToSpotLight = dot(-lightDir, vec3(0.0, -1.0, 0.0));
    float cosSpotlightAngle = 0.8;

    if(angleToSpotLight > cosSpotlightAngle)
        pointLight = rainResponse(pointLightDir, vec3(pointLightColor), 2 * pointLightIntensity * randEnlight, true);

    pointLight = rainResponse(pointLightDir, vec3(pointLightColor), 2 * pointLightIntensity * randEnlight, true);

    FragColor = (texture(rainTex, fragmentTexCoords.xy)) * vec4(lightFunc(), 1.0);
    FragColor.a *= 0.7 * u_alpha;
}
