#version 330

uniform vec4 u_color;
out vec4 FinalColor;

in vec3 normal;
in vec3 pos;

void main() {
    //FinalColor = vec4(1.0, 0.0, 1.0, 1.0);
    FinalColor = vec4(normal,1.0);
}