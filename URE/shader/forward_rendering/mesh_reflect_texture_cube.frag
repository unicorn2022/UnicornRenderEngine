#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;
in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 ViewPosition;
} fs_in;

/* uniform 变量 */
// 天空盒纹理
uniform samplerCube texture_cube;

void main() {
    vec3 I = normalize(fs_in.Position - fs_in.ViewPosition);
    vec3 R = reflect(I, normalize(fs_in.Normal));
    vec3 color = texture(texture_cube, R).rgb;
    
    FragColor = vec4(color, 1.0);
}