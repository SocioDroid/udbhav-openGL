#version 460 core

in vec4 a_position_out;
in vec2 a_texcoord_out;  
in vec3 a_normal_out;

/*
in vec3 out_lightDirection;
in vec3 out_halfVector;
in float out_attenuation;
*/

//uniform vec3 LightPosition;

uniform vec3 u_la[4];
uniform vec3 u_ld[4];
uniform vec3 u_ls[4];
uniform vec4 u_lightPosition[4];

uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_shininess;

uniform float u_strength;

uniform vec3 u_eyeDirection;
uniform float u_constantAttenuation;    // Spot Light     
uniform float u_linearAttenuation;      // Spot Light 
uniform float u_quadraticAttenuation;   // Spot Light

uniform vec3 u_coneDirection;   //Adding spotlight attributes
uniform float u_spotCosCutoff;  //width of the spot
uniform float u_spotExponent;   //control light fall-off in the spot

uniform int u_lightingEnabled;

uniform sampler2D  u_texturesampler;

out vec4 fragColor;

void main(void)
{
    vec4 texture_color;
    texture_color = texture( u_texturesampler, a_texcoord_out);

    if(u_lightingEnabled == 1)
    {
        vec3 scatteredLight[4];
        vec3 reflectedLight[4];
        vec3 ambientColor[4];
        vec3 diffuseColor[4];
        vec3 specularColor[4];
        vec3 lightColor[4];
        vec3 lightDirection[4];
        vec3 rgb;
        vec3 halfVector;
        float diffuse;
        float specular;

        for(int i = 0; i < 4; i++)
        {
            lightDirection[i] = normalize(vec3(u_lightPosition[i]) - vec3(a_position_out));

            float lightDistance = length(lightDirection[i]);
            lightDirection[i] = lightDirection[i] / lightDistance;

            float attenuation = 1.0 / (u_constantAttenuation + (u_linearAttenuation * lightDistance) + (u_quadraticAttenuation * lightDistance * lightDistance));

            //Distance in spotlight
            float cosOfSpot = dot(lightDirection[i], -u_coneDirection);

            //Attenuate more, based on spot-related position
            if(cosOfSpot < u_spotCosCutoff)
                attenuation = 0.0;  // Sharp SpotLight
            else    
                attenuation = attenuation * pow(cosOfSpot, u_spotExponent);

            halfVector = normalize(lightDirection[i] + u_eyeDirection);
                
            diffuse = max(dot(a_normal_out, lightDirection[i]), 0.0);
            specular = max(dot(a_normal_out, halfVector), 0.0);
        

            scatteredLight[i] = (u_la[i] * u_ka) + u_ld[i] * diffuse * attenuation;
            reflectedLight[i] = u_ld[i] * specular * attenuation;

            ambientColor[i] = u_la[i] * u_ka;
            diffuseColor[i] = u_ld[i] * u_kd * diffuse;
            specularColor[i] = u_ls[i] * u_ks * specular;
            lightColor[i] = ambientColor[i] + diffuseColor[i] + specularColor[i];

            // vec3 rgb = normalize((lightColor * scatteredLight) + reflectedLight);
            rgb = rgb + (min(lightColor[i] * scatteredLight[i] + reflectedLight[i], vec3(1.0, 1.0, 1.0)));
        }

        if(diffuse == 0.0)
            specular = 0.0;
        else
            specular = pow(specular, u_shininess) * u_strength; 


        if(texture_color[0] <= 0.0 && texture_color[1] <= 0.0 && texture_color[2] <= 0.1)
            fragColor = (vec4(rgb, 1.0)) ;
        else    
            fragColor = (vec4(rgb, 1.0)) * texture_color;

    }
    else
    {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}


