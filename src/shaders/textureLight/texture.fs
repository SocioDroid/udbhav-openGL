#version 460 core

in vec2 a_texcoords_out;
in vec4 phong_vertex_position;
in vec3 phong_vertex_normal_vector;
in vec3 phong_vertex_color;

uniform sampler2D u_textureSampler;
uniform float u_alphaVal = 1.0;
uniform float lightX;
uniform float lightY;
uniform float lightZ;

out vec4 FragColor;

void main(void) {
     // LIGHT
    /* Distance to light source. */
    const float light_distance = 5.0;

    vec3 light_location = vec3(lightX, lightY, -1.0 + lightZ);

    /* Scene ambient color. */
    const vec3 ambient_color = vec3(0.1, 0.1, 0.1);
    const float attenuation = 1.0;
    const float shiness = 1.0;

    /* Normalize directions. */
    vec3 normal_direction = normalize(phong_vertex_normal_vector);
    vec3 view_direction = normalize(vec3(vec4(0.0, 0.0, -5.0, 0.0) - phong_vertex_position));
    vec3 light_direction = normalize(light_location);

    /** Calculate ambient lighting component of directional light. */
    vec3 ambient_lighting = ambient_color * phong_vertex_color;

    /** Calculate diffuse reflection lighting component of directional light. */
    vec3 diffuse_reflection = attenuation * phong_vertex_color * max(0.0, dot(normal_direction, light_direction));

    /** Calculate specular reflection lighting component of directional light. */
    vec3 specular_reflection = vec3(0.0, 0.0, 0.0);

    if(dot(normal_direction, light_direction) >= 0.0) {
        /* Light source on the right side. */
        specular_reflection = attenuation * phong_vertex_color * pow(max(0.0, dot(reflect(-light_direction, normal_direction), view_direction)), shiness);
    }

    /** Calculate fragment lighting as sum of previous three component. */
    vec4 FragColorLight = vec4(ambient_lighting + diffuse_reflection + specular_reflection, 1.0);

    vec4 texColor = texture(u_textureSampler, a_texcoords_out);
    FragColor = FragColorLight * vec4(texColor.xyz, texColor.a * u_alphaVal);
}
