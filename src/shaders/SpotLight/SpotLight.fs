#version 440 core

in vec3 a_normal_out;

in vec4 a_position_out;
in vec2 a_texcoord_out;  

/*
in vec3 out_lightDirection;
in vec3 out_halfVector;
in float out_attenuation;
*/

//uniform vec3 LightPosition;

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

uniform sampler2D  u_texturesampler;

out vec4 fragColor;

void main(void)
{
    vec4 texture_color;
    texture_color = texture( u_texturesampler, a_texcoord_out);

    if(u_lightingEnabled == 1)
    {
        vec3 lightDirection = normalize(u_lightPosition - vec3(a_position_out));

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
        vec3 lightColor = ambientColor + diffuseColor + specularColor;

        vec3 rgb = min(lightColor * scatteredLight + reflectedLight, vec3(1.0, 1.0, 1.0));

        if(texture_color[0] <= 0.0 && texture_color[1] <= 0.0 && texture_color[2] <= 0.1)
            fragColor = (vec4(rgb, 1.0)) ;
        else    
            fragColor = (vec4(rgb, 1.0)) * texture_color ;

    }
    else
    {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}


