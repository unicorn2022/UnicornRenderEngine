#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;
in vec3 local_position;

/* uniform 变量 */
uniform vec4 color;

void main() {
    // 绿色(点光源可视化颜色)
    if (color == vec4(0.0f, 1.0f, 0.0f, 1.0f)) {
        FragColor = color;
    } else {
        // x轴为指向轴
        if (local_position.x > 0.5f) FragColor = color;
        else FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}