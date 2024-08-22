#version 460 core

/* 输入输出变量 */
in vec4 FragPos;
out vec4 FragColor;

/* uniform 变量 */
uniform vec3 light_position;

const float POINT_LIGHT_SHADOW_ZFAR = 100.0f;

void main() {
    float light_distance = length(FragPos.xyz - light_position) / POINT_LIGHT_SHADOW_ZFAR;
    
    gl_FragDepth = light_distance;
    FragColor = vec4(light_distance, 0.0, 0.0, 1.0);
}