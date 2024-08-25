#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;

void main() {
    FragColor = vec4(gl_FragCoord.z, 0.0f, 0.0f, 1.0f);
}