#version 330

uniform vec4 u_color;

out vec4 FinalColor;

void main() {
    //FinalColor = vec4(1.0, 0.0, 1.0, 1.0);
    FinalColor = u_color;
}