#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;

/* uniform 变量 */
uniform vec4 color;

void main() {
    FragColor = color;
}