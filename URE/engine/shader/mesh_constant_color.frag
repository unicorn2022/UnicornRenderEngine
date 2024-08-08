#version 330 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

/* uniform 变量 */
uniform vec4 color;

void main() {
    FragColor = color;
}