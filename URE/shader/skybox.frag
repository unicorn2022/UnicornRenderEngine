#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 ViewDirection;
} fs_in;

uniform samplerCube skybox_texture;

void main() {
    FragColor = texture(skybox_texture, fs_in.ViewDirection);
}