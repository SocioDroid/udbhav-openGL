#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

in vec4 a_position_out;

in vec3 a_normal_out;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

// Spot light
uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec3 u_lightPosition;

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_shininess;

uniform float u_strength;

uniform vec3 u_eyeDirection;
uniform float u_constantAttenuation;
uniform float u_linearAttenuation;
uniform float u_quadraticAttenuation;

uniform vec3 u_coneDirection;//Adding spotlight attributes
uniform float u_spotCosCutoff;//width of the spot
uniform float u_spotExponent;//control light fall-off in the spot

uniform int u_lightingEnabled;

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
    vec3 normal = normalize(Normal);
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

void main()
{           
    // vec3 color = texture(diffuseTexture, TexCoords).rgb;

    // vec3 normal = normalize(Normal);
    // vec3 lightColor = vec3(0.5);
    // // ambient
    // vec3 ambient = 0.4 * lightColor;
    // // diffuse
    // vec3 lightDir = normalize(lightPos - FragPos);
    // float diff = max(dot(lightDir, normal), 0.0);
    // vec3 diffuse = diff * lightColor;
    // // specular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = 0.3;
    // vec3 halfwayDir = normalize(lightDir + viewDir);  
    // spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    // vec3 specular = spec * lightColor;    

        // vec3 normal = normalize(Normal);

        vec4 texture_color;
        texture_color = texture( diffuseTexture, TexCoords);

        vec3 lightDirection = u_lightPosition - vec3(a_position_out);
        float lightDistance = length(lightDirection);
        lightDirection = lightDirection / lightDistance;

        float attenuation = 1.0 / (u_constantAttenuation + (u_linearAttenuation * lightDistance) + (u_quadraticAttenuation * lightDistance * lightDistance));

        //Distance in spotlight
        float cosOfSpot = dot(lightDirection, -u_coneDirection);

        //Attenuate more, based on spot-related position
        if(cosOfSpot < u_spotCosCutoff)
            attenuation = 0.0;
        else    
            attenuation = attenuation * pow(cosOfSpot, u_spotExponent);

        vec3 halfVector = normalize(lightDirection + u_eyeDirection);
            
        float diffuse = max(dot(a_normal_out, lightDirection), 0.0);
        float specular = max(dot(a_normal_out, halfVector), 0.0);
        
        if(diffuse == 0.0)
            specular = 0.0;
        else
            specular = pow(specular, u_shininess) * u_strength; 

        vec3 scatteredLight = (u_la * u_ka) + u_ld * diffuse * attenuation;
        vec3 reflectedLight = u_ld * specular * attenuation;

        vec3 ambientColor = u_la * u_ka;
        vec3 diffuseColor = u_ld * u_kd * diffuse;
        vec3 specularColor = u_ls * u_ks * specular;

         // calculate shadow
        float shadow = ShadowCalculation(FragPosLightSpace);     

        vec3 lightColor = (ambientColor + (1.0 - shadow) * diffuseColor + specularColor) ;
        // vec3 lightColor = (ambientColor + diffuseColor + specularColor) ;

        vec3 rgb = min(lightColor * scatteredLight + reflectedLight, vec3(1.0, 1.0, 1.0));

        if(texture_color[0] <= 0.0 && texture_color[1] <= 0.0 && texture_color[2] <= 0.1)
            FragColor = (vec4(rgb, 1.0)) ;
        else    
            FragColor = (vec4(rgb, 1.0)) * texture_color ;



    //vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    // FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0);
}
