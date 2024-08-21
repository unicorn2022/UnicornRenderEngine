#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;
in vec3 local_position;

/* uniform 变量 */
uniform vec4 color;

void main() {
    FragColor = color;
    // x轴为指向轴
    if (local_position.x > 0.5f) FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    else FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}