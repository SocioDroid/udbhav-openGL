#version 460 core

in vec2 a_texcoord_tes_out;
in vec3 FragPos_tes_out;
in vec3 TangentLightPos_tes_out;
in vec3 TangentViewPos_tes_out;
in vec3 TangentFragPos_tes_out;
in vec3 DOF_View_transformed;

// TERRAIN RELATED UNIFORMS
uniform sampler2D heightMap;
uniform sampler2D texture_background;
uniform sampler2D normalMap;

uniform float u_alpha;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int isBarsat = 0;

out vec4 FragColor;

vec2 ParallaxMapping(vec2 texCo, vec3 viewDr)
{
float height = texture(heightMap, texCo).r;
return(texCo - viewDr.xy * (height * 0.2));
}

void main(void)
{
    // Light Related   
    vec3 viewDir = normalize(TangentViewPos_tes_out - TangentFragPos_tes_out);
    vec2 texCoords = a_texcoord_tes_out;
	//texCoords = ParallaxMapping(texCoords, viewDir);


    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // diffuse color
    vec3 color;
    if(isBarsat == 1)
    {
    color = texture(texture_background, texCoords).rgb * vec3(0.7 , 0.7, 0.7) ;
    color = mix(color ,  vec3(0.69411, 0.4980,0.364) , 0.2); 
    color = color *  vec3(0.6 , 0.6, 0.6); 
    }
    else
    {
        color = texture(texture_background, texCoords).rgb;
    }
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos_tes_out - TangentFragPos_tes_out);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    //vec3 viewDir = normalize(TangentViewPos_tes_out - TangentFragPos_tes_out);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);

    //  FragColor = vec4(color, DOF_View_transformed.z) ;
    // FragColor = vec4(1.0,1.0,1.0, 1.0);
}
