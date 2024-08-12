#version 330 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

/* uniform 变量 */
// 观察位置
uniform vec3 view_position;
// 天空盒纹理
uniform samplerCube skybox;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - view_position);
    vec3 R = refract(I, normalize(Normal), ratio);
    vec3 color = texture(skybox, R).rgb;
    FragColor = vec4(color, 1.0);
}