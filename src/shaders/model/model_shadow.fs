#version 440 core

in vec2 a_texcoord_out;  
in vec3 transformedNormals;
in vec3 lightDirection;
in vec3 viewerVector;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform float u_materialShininnes;
uniform int u_lightingEnabled;
uniform int disableShadow =0;
uniform float u_alpha = 1.0;

layout(binding = 0) uniform sampler2D  texture_diffuse1;
layout(binding = 4) uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(transformedNormals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}


void main(void)
{
    vec3 phong_ads_color;
    vec4 texture_color;
    texture_color = texture( texture_diffuse1, a_texcoord_out);

    float shadow = ShadowCalculation(FragPosLightSpace);                      
    
            vec3 ambiant = u_la * u_ka;
            vec3 normalized_transformed_normals = normalize(transformedNormals);
            vec3 normalized_light_direction = normalize(lightDirection);
            vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction ,normalized_transformed_normals),0.0);
            vec3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);
            vec3 normalized_view_vector = normalize(viewerVector);
            vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , normalized_view_vector), 0.0), u_materialShininnes);
            
            // phong_ads_color = (ambiant + diffuse +  specular);
            // shadow = mix(shadow, vec3(1.0, 1.0 , 1.0), 0.5);

            if(texture_color[0] <= 0.0 && texture_color[1] <= 0.0 && texture_color[2] <= 0.1)
            {
                if(disableShadow == 0) 
                    phong_ads_color = (ambiant + (1.0 - shadow) * (diffuse +  specular));
                else
                     phong_ads_color = (ambiant + diffuse +  specular);
            }
            else   
            {
            if(disableShadow == 0) 
                //phong_ads_color = (ambiant + diffuse +  specular) * vec3(texture_color) ; 
                phong_ads_color = (ambiant + (1.0 - shadow) * (diffuse +  specular)) * vec3(texture_color) ;
            else
                phong_ads_color = (ambiant + diffuse +  specular) * vec3(texture_color) ;
            }
    
    if(phong_ads_color.x <= 0.001 && phong_ads_color.y <= 0.001 && phong_ads_color.z <= 0.001)
    {
        FragColor = vec4(0.0,0.0,0.0, 0.0);
    }
    else
    {
        FragColor = vec4(phong_ads_color,u_alpha);
        //FragColor = vec4(0.0f, 1.0f, 0.0f,1.0);
    }

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.1)
       BrightColor = vec4(FragColor.rgb, 1.0);
    else
       BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
