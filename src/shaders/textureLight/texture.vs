#version 460 core

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 a_texcoords_out;
out vec4 phong_vertex_position;      /**< position of the vertex in world space.  */
out vec3 phong_vertex_normal_vector; /**< surface normal vector in world space.   */
out vec3 phong_vertex_color;

void main(void) {
	// Position
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;

	// Light
	phong_vertex_position = a_position;                                /* Set vertex position for fragment shader.          */
	phong_vertex_normal_vector = a_normal;                              /* Set normal vector to surface for fragment shader. */
	phong_vertex_color = vec3(0.7);

	// Texture
	a_texcoords_out = a_texcoord;
}
