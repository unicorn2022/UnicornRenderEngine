#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
in VS_OUT {
    vec3 Position;
} fs_in;
out vec4 FragColor;

uniform vec3 light_position;
const float POINT_LIGHT_SHADOW_ZFAR = 100.0f;

void main() {
    float dist = length(fs_in.Position - light_position) / POINT_LIGHT_SHADOW_ZFAR;
    FragColor = vec4(dist, 0.0f, 0.0f, 1.0f);
}