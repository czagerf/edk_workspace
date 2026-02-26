#version 330

uniform vec4 u_color;
uniform sampler2D u_texture_00;
uniform sampler2D u_texture_01;
uniform sampler2D u_texture_02;

in vec2 uv;

out vec4 FinalColor;

void main() {
    FinalColor = texture(u_texture_00,uv)*0.33+texture(u_texture_01,uv)*0.33+texture(u_texture_02,uv)*0.33;
}