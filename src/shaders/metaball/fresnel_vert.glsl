out float vertexFactor;

void main() {
    vec3 worldNormal = (view * model * vec4(normal, 0.0f)).xyz;
    vertexFactor = dot(normalize(worldNormal), normalize(viewAngle));
    
    vec3 offset = normal * 1.0f;
    gl_Position = projection * view * model * vec4(position + offset, 1.0f);
}