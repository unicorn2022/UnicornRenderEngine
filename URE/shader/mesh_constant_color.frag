#version 330 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;

in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

/* uniform 变量 */
uniform vec4 color;

void main() {
    FragColor = color;
}