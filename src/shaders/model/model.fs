#version 460 core

in vec2 a_texcoord_out;
in vec3 transformedNormals;
in vec3 lightDirection;
in vec3 viewerVector;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec3 u_ka;
uniform vec3 u_ks;
uniform vec3 u_kd;
uniform float u_materialShininnes;
uniform float u_alpha = 1.0;

uniform float u_fade_alpha = 1.0;

uniform int normalMappingEnabled;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform bool noTexture = false;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

void main(void) {
    vec3 phong_ads_color;
    vec4 texture_color;
    texture_color = texture(texture_diffuse1, a_texcoord_out);
    vec3 normal = texture(texture_specular1, a_texcoord_out).rgb;

    if(normalMappingEnabled == 0) // normalMappingEnabled = FALSE
    {
        vec3 ambiant = u_la * u_ka;
        vec3 normalized_transformed_normals = normalize(transformedNormals);
        vec3 normalized_light_direction = normalize(lightDirection);
        vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0);
        vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals);
        vec3 normalized_view_vector = normalize(viewerVector);
        vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_view_vector), 0.0), u_materialShininnes);

            // phong_ads_color = (ambiant + diffuse +  specular);
        if(noTexture)
            phong_ads_color = u_kd;
        else
            phong_ads_color = (ambiant + diffuse + specular) * vec3(texture_color);

    } else // normalMappingEnabled = TRUE
    {
        normal = normalize(normal * 2.0 - 1.0);

        vec3 ambiant = u_la * u_ka;// ambiant color milala

            // diffuse color milala
        vec3 diffuse_color = u_ld * u_kd;

        vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * diffuse_color;

            // specular
        vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

        vec3 specular = vec3(u_ls * u_ks) * spec;

            // phong_ads_color = (ambiant + diffuse +  specular);
        if(noTexture)
            phong_ads_color = (ambiant + diffuse + specular);
        else
            phong_ads_color = (ambiant + diffuse + specular) * vec3(texture_color);
    }

    if(phong_ads_color.x <= 0.001 && phong_ads_color.y <= 0.001 && phong_ads_color.z <= 0.001) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    } else {
        // FragColor = vec4(phong_ads_color,v_DOF.z); // Enable For DOF
        FragColor = (vec4(phong_ads_color, u_alpha)) * u_fade_alpha;
    }

    // float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    // if(brightness > 0.1)
    //    BrightColor = vec4(FragColor.rgb, 1.0);
    // else
    //    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
