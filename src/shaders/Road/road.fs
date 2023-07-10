#version 460 core

in vec2 a_texcoords_out;
in vec3 lightDirection;
in vec3 viewerVector;
in vec3 transformedNormals;

uniform sampler2D u_textureSampler;
uniform sampler2D u_textureNormalSampler;

uniform vec3 u_la;
uniform vec3 u_ld;
uniform vec3 u_ls;
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform float u_materialShininess;

out vec4 FragColor;

void main(void)
{
    vec3 phong_ads_light;

    vec3 ambient = u_la * u_ka;
	// vec3 normalized_transformed_normals = normalize(transformedNormals);
	vec3 normalized_transformed_normals = normalize(texture(u_textureNormalSampler, a_texcoords_out).xyz * 2.0f - 1.0f);
	
	vec3 normalized_light_direction = normalize(lightDirection);
	vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0);
	vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals);
	vec3 normalized_viewerVector = normalize(viewerVector);
	vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewerVector), 0.0), u_materialShininess);
	phong_ads_light = ambient + diffuse + specular;

    FragColor = texture(u_textureSampler, a_texcoords_out);
    FragColor = FragColor * vec4(phong_ads_light, 1.0);
    // FragColor = vec4(1.0);
}
