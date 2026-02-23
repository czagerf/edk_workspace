#version 330
uniform mat4 u_vp_matrix;
uniform mat4 u_m_matrix;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_uv;

out vec3 normal;
out vec3 pos;

void main() {
    normal = a_normal;
    gl_Position = u_vp_matrix * u_m_matrix * vec4(a_position, 1.0);
}